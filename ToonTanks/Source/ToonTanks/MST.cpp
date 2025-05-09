// Fill out your copyright notice in the Description page of Project Settings.


#include "MST.h"
#include "Grid.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"



MST::MST()
{
}

MST::MST(std::vector<double> delaunaycoords, std::vector<std::size_t> delaunaytriangles,TMap<FVector2D, class AActor*> lookupcorrdinatestoroomtable)
{
    DelaunayCordinates = delaunaycoords;
    DelaunayTriangles = delaunaytriangles;
    LookUpCorrdinatesToRoomTable = lookupcorrdinatestoroomtable;
    //PrintTriangles();
    BuildEdgesMap();
    
    //Call Prim's Algorithm
    MinimumSpanTree();
}

MST::~MST()
{
}

void MST::PrintTriangles()
{
    for(std::size_t i = 0; i < DelaunayTriangles.size(); i+=3) {
        UE_LOG(LogTemp, Log, TEXT("INSIDE MST :: Triangle points: [[%f, %f], [%f, %f], [%f, %f]]"),
            DelaunayCordinates[2 * DelaunayTriangles[i]],        //tx0
            DelaunayCordinates[2 * DelaunayTriangles[i] + 1],    //ty0
            DelaunayCordinates[2 * DelaunayTriangles[i + 1]],    //tx1
            DelaunayCordinates[2 * DelaunayTriangles[i + 1] + 1],//ty1
            DelaunayCordinates[2 * DelaunayTriangles[i + 2]],    //tx2
            DelaunayCordinates[2 * DelaunayTriangles[i + 2] + 1] //ty2
        );        
    }
    //Iterate through TMap and print the coordinates
    for (auto& Elem : LookUpCorrdinatesToRoomTable)
    {
        FVector2D Key = Elem.Key;
        AActor* Value = Elem.Value;
        UE_LOG(LogTemp, Log, TEXT("Key: %f, %f Value: %s"), Key.X, Key.Y, *(Value->GetFName().ToString()));
    }
}

void MST::BuildEdgesMap()
{
    for(std::size_t i=0; i < DelaunayTriangles.size();i+=3)
    {
        //Get Room1
        AActor* Room1 = LookUpCorrdinatesToRoomTable[{DelaunayCordinates[2 * DelaunayTriangles[i]], DelaunayCordinates[2 * DelaunayTriangles[i] + 1]}];
        if(MapOfEdges.Contains(Room1) == false)
        {
            MapOfEdges.Add(Room1, TSet<AActor*>());
        }
        //Get Room2
        AActor* Room2 = LookUpCorrdinatesToRoomTable[{DelaunayCordinates[2 * DelaunayTriangles[i + 1]], DelaunayCordinates[2 * DelaunayTriangles[i + 1] + 1]}];
        if(MapOfEdges.Contains(Room2) == false)
        {
            MapOfEdges.Add(Room2, TSet<AActor*>());
        }
        //Get Room3
        AActor* Room3 = LookUpCorrdinatesToRoomTable[{DelaunayCordinates[2 * DelaunayTriangles[i + 2]], DelaunayCordinates[2 * DelaunayTriangles[i + 2] + 1]}];
        if(MapOfEdges.Contains(Room3) == false)
        {
            MapOfEdges.Add(Room3, TSet<AActor*>());
        }
        //Add Room1<->Room2 Edge
        MapOfEdges[Room1].Add(Room2);
        MapOfEdges[Room2].Add(Room1);
        //Add Room2<->Room3 Edge
        MapOfEdges[Room2].Add(Room3);
        MapOfEdges[Room3].Add(Room2);
        //Add Room3<->Room1 Edge
        MapOfEdges[Room3].Add(Room1);
        MapOfEdges[Room1].Add(Room3);
    }

    DebugPrintEdgesMap();
    
}

void MST::DebugPrintEdgesMap()
{
    for (auto& Elem : MapOfEdges)
    {
        AActor* Key = Elem.Key;
        TSet<AActor*> Value = Elem.Value;
        UE_LOG(LogTemp, Log, TEXT("Key: %s"),*(Key->GetFName().ToString()));
        for(AActor* Room : Value)
        {
            UE_LOG(LogTemp, Log, TEXT("Value: %s"),*(Room->GetFName().ToString()));
        }
    }    
}

void MST::ResetVisitedRooms()
{
    for(auto& Elem : LookUpCorrdinatesToRoomTable)
    {
        UnvisitedRooms.Add(Elem.Value);
    }
    VisitedRooms.Empty(UnvisitedRooms.Num());
    RoomsInPQ.Empty(UnvisitedRooms.Num());
}

void MST::MinimumSpanTree()
{
    //Restart Set with visited, unvisted and currently in PQ rooms.
    ResetVisitedRooms();

    //Pick a random room to start with
    AActor* StartRoom = UnvisitedRooms.Array()[0];
    PQHeap.HeapPush(FHeapElements(StartRoom,0,nullptr));
    RoomsInPQ.Add(StartRoom);
    UnvisitedRooms.Remove(StartRoom);

    while(PQHeap.Num() > 0)
    {
        //Get the closest Room to one of the Rooms already in the graph.
        FHeapElements PoppedElement;
        PQHeap.HeapPop(PoppedElement);
        AActor* CurrRoom = PoppedElement.RoomToVisit;
        RoomsInPQ.Remove(CurrRoom);
        VisitedRooms.Add(CurrRoom);
        MSTBuilt.Add(PoppedElement);
        MSTFinalEdges.Add(TTuple<AActor*, AActor*>(PoppedElement.RoomToVisit, PoppedElement.FromThisRoom));
        //Add all the neighbors of the popped room to the PQ.
        //  If already inside, compare the cost and update the cost if closer than the one in the PQ. Update FromRoom as well.
        //  If not inside, add it to the PQ.
        for(AActor* Neighbor : MapOfEdges[CurrRoom])
        {
            float CurrDistanceToNeighbor = FMath::Abs(FVector::Dist(CurrRoom->GetActorLocation(), Neighbor->GetActorLocation()));
            if(RoomsInPQ.Contains(Neighbor))
            {
                //Compare the cost and update the cost if closer than the one in the PQ. Update FromRoom as well.
                for(int ind = 0; ind < PQHeap.Num(); ind++)
                {
                    if(PQHeap[ind].RoomToVisit == Neighbor)
                    {
                        if(PQHeap[ind].Cost > CurrDistanceToNeighbor)
                        {
                            UnusedEdges.Add(TTuple<AActor*, AActor*>(Neighbor, PQHeap[ind].FromThisRoom));
                            PQHeap[ind].Cost = CurrDistanceToNeighbor;
                            PQHeap[ind].FromThisRoom = CurrRoom;
                            
                        }
                        //This means that the Neighbor is already in the PQ and has a better path already.
                        else
                        {
                            UnusedEdges.Add(TTuple<AActor*, AActor*>(Neighbor, CurrRoom));
                        }
                    }
                }
            }
            else if(UnvisitedRooms.Contains(Neighbor))
            {
                //Add it to the PQ.
                
                PQHeap.HeapPush(FHeapElements(Neighbor,CurrDistanceToNeighbor,CurrRoom));
                RoomsInPQ.Add(Neighbor);
                UnvisitedRooms.Remove(Neighbor);
            }
            else if(VisitedRooms.Contains(Neighbor))
            {
                //UnusedEdges.Add(TTuple<AActor*, AActor*>(Neighbor, CurrRoom));
            }
        }
    }

    //DebugPrintMSTBuilt();
    DebugPrintUnusedEdges();
    PickRandomUnusedEdge();

}

void MST::DebugPrintMSTBuilt()
{
    for(FHeapElements Elem : MSTBuilt)
    {
        UE_LOG(LogTemp, Log, TEXT("RoomToVisit: %s, Cost: %f, FromThisRoom: %s"), *(Elem.RoomToVisit->GetFName().ToString()), Elem.Cost, Elem.FromThisRoom == nullptr ? TEXT("nullptr") : *(Elem.FromThisRoom->GetFName().ToString()));
    }
}

TArray<TTuple<AActor*, AActor*>> MST::GetMSTFinalEdges() const
{
    return MSTFinalEdges;
}

void MST::DebugPrintUnusedEdges()
{
    for(TTuple<AActor*, AActor*> Elem : UnusedEdges)
    {
        UE_LOG(LogTemp, Log, TEXT("Unused Edges. From Room1: %s, to Room2: %s"), *(Elem.Get<0>()->GetFName().ToString()),Elem.Get<1>() == nullptr? TEXT("nullptr") : *(Elem.Get<1>()->GetFName().ToString()));
    }
}

void MST::PickRandomUnusedEdge()
{
    if(UnusedEdges.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No Unused Edges to pick from"));
        return;
    }
    for(TTuple<AActor*, AActor*> Elem : UnusedEdges)
    {
        int RandomNum = FMath::RandRange(0,100);
        if(RandomNum / 100.0f < PickUnusedEdgeProbability)
        {
            MSTFinalEdges.Add(Elem);
        }
    }
}
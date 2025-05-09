// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "RoomGenerator.h"
#include "DrawDebugHelpers.h"
#include "MST.h"
#include "Delaunay/delaunator.hpp"
#include "HallwayGenerator.h"
using namespace std;

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	GridArray.Init(0,LenX * LenY);
	ARoomGenerator*  RoomGenerator = GetWorld()->SpawnActor<ARoomGenerator>(RoomGeneratorClass,GetActorLocation(),GetActorRotation());
	if(!RoomGenerator)
	{
		UE_LOG(LogTemp, Error, TEXT("Room Generator is not spawned"));
		return;
	}
	RoomGenerator->SetOwner(this);
	RoomGenerator->Generate();
	DisplayDebugGrid();
	DelaunayTriangulation();
	MST MinimumSpanningTree = MST::MST(DelaunayCordinates, DelaunayTriangles,GetLookUpCorrdinatesToRoomTable());
	MSTFinalEdges = MinimumSpanningTree.GetMSTFinalEdges();
	DisplayMSTTree();
	AHallwayGenerator* HallwayGenerator = GetWorld()->SpawnActor<AHallwayGenerator>(HallwayGeneratorClass,GetActorLocation(),GetActorRotation());
	if(!HallwayGenerator)
	{
		UE_LOG(LogTemp, Error, TEXT("Hallway Generator is not spawned"));
		return;
	}
	HallwayGenerator->SetOwner(this);
	HallwayGenerator->GenerateHallways();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called every frame
void AGrid::DisplayDebugGrid()
{
	if (LenX <= 0 || LenY <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid size is less than or equal to 0"));
		return;
	}

	//FVector2D StartLocation = FVector2D(GetActorLocation().X - (LenX * CellSize) / 2, GetActorLocation().Y - (LenY * CellSize) / 2);
	//FVector CurrentLocation = FVector(StartLocation.X, StartLocation.Y, GetActorLocation().Z);
	FVector StartLocation = GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	for (int i = 0; i < LenX; i++)
	{
		for (int j = 0; j < LenY; j++)
		{
			FColor DebugColor; 
			if(GridArray[i * LenY + j ] == 0)
			{
				DebugColor = FColor::Emerald;
			}
			else
			{
				DebugColor = FColor::Red;
			}
			DrawDebugBox(GetWorld(), CurrentLocation, FVector(CellSize/2, CellSize/2, CellSize/2), DebugColor, true, -1, 0, 5);			
			CurrentLocation.Y += CellSize;
		}
		CurrentLocation.Y = StartLocation.Y;
		CurrentLocation.X += CellSize;
	}
}

FVector2D AGrid::GetGridRowCol() const
{
	return FVector2D(LenX - 1, LenY - 1);	
}

void AGrid::PlaceRoomAtCoordinate(int X, int Y)
{
	GridArray[(X) * LenY + (Y)] = 1;
	FVector SpawnLocation = GetActorLocation() + FVector(X * CellSize, Y * CellSize, 0);
	AActor*  Room = GetWorld()->SpawnActor<AActor>(RoomClass,SpawnLocation,GetActorRotation());
	if(!Room)
	{
		UE_LOG(LogTemp, Error, TEXT("Room is not spawned"));
		return;
	}
	RoomsPlacedArray.Add(Room);
}


int AGrid::GetOccupiedCell(int X, int Y) const
{
	return GridArray[(X) * LenY + (Y)];
}

void AGrid::DelaunayTriangulation()
{
	TArray<double> InputCoordinates = ConstructDelaunayInputArray();
	std::vector<double> InputCoordinatesVector(InputCoordinates.GetData(), InputCoordinates.GetData() + InputCoordinates.Num());
	delaunator::Delaunator d(InputCoordinatesVector);
	for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
		UE_LOG(LogTemp, Log, TEXT("Triangle points: [[%f, %f], [%f, %f], [%f, %f]]"),
			d.coords[2 * d.triangles[i]],        //tx0
			d.coords[2 * d.triangles[i] + 1],    //ty0
			d.coords[2 * d.triangles[i + 1]],    //tx1
			d.coords[2 * d.triangles[i + 1] + 1],//ty1
			d.coords[2 * d.triangles[i + 2]],    //tx2
			d.coords[2 * d.triangles[i + 2] + 1] //ty2
		);
		//Edge X0->X1
		DrawDebugLine(GetWorld(), FVector(d.coords[2 * d.triangles[i]],d.coords[2 * d.triangles[i] + 1], 0), FVector(d.coords[2 * d.triangles[i + 1]], d.coords[2 * d.triangles[i + 1] + 1], 0), FColor::Green, true, -1, 0, 5);
		//Edge X0->X2
		DrawDebugLine(GetWorld(), FVector(d.coords[2 * d.triangles[i]],d.coords[2 * d.triangles[i] + 1], 0), FVector(d.coords[2 * d.triangles[i + 2]], d.coords[2 * d.triangles[i + 2] + 1], 0), FColor::Green, true, -1, 0, 5);
		//Edge X1->X2
		DrawDebugLine(GetWorld(), FVector(d.coords[2 * d.triangles[i + 1]], d.coords[2 * d.triangles[i + 1] + 1], 0), FVector(d.coords[2 * d.triangles[i + 2]], d.coords[2 * d.triangles[i + 2] + 1], 0), FColor::Green, true, -1, 0, 5);
    }
	DelaunayCordinates = d.coords;
	DelaunayTriangles = d.triangles;
}

TArray<double> AGrid::ConstructDelaunayInputArray()
{
	TArray<double> InputCoordinates;
	for(AActor* Room : RoomsPlacedArray)
	{
		FVector RoomLocation = Room->GetActorLocation();
		InputCoordinates.Add(RoomLocation.X);
		InputCoordinates.Add(RoomLocation.Y);
	}
	return InputCoordinates;
}

void AGrid::DebugPrintArray(TArray<double> Array)
{
	for(int i = 0; i < Array.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Array[%d] = %f"), i, Array[i]);
	}
}

TMap<FVector2D, class AActor*> AGrid::GetLookUpCorrdinatesToRoomTable() const
{
	TMap<FVector2D, class AActor*> LookUpCorrdinatesToRoomTable;
	for(AActor* Room : RoomsPlacedArray)
	{
		FVector RoomLocation = Room->GetActorLocation();
		LookUpCorrdinatesToRoomTable.Add({FVector2D(RoomLocation.X, RoomLocation.Y), Room});
	}
	if(LookUpCorrdinatesToRoomTable.Num() != RoomsPlacedArray.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("LookUpCorrdinatesToRoomTable size is not equal to RoomsPlacedArray size"));
	}
	return LookUpCorrdinatesToRoomTable;
}

void AGrid::DisplayMSTTree()
{
	for(TTuple<AActor*, AActor*> Elem : MSTFinalEdges)
	{
		
		UE_LOG(LogTemp, Log, TEXT("Room1: %s, Room2: %s"), *(Elem.Get<0>()->GetFName().ToString()),Elem.Get<1>() == nullptr? TEXT("nullptr") : *(Elem.Get<1>()->GetFName().ToString()));
		if(Elem.Get<1>())
		{
			DrawDebugLine(GetWorld(), Elem.Get<0>()->GetActorLocation(), Elem.Get<1>()->GetActorLocation(), FColor::Red, true, -1, 0, 15);
		}
	}
}
TArray<TTuple<AActor*, AActor*>> AGrid::GetMSTFinalEdges() const
{
	return MSTFinalEdges;
}

float AGrid::GetCellSize() const
{
	return CellSize;
}
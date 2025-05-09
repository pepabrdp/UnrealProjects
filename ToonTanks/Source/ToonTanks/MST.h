// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include <set>
#include <tuple>
#include <queue>

using namespace std;

/**
 * 
 */


struct FHeapElements
{
	float Cost;
	AActor* RoomToVisit;
	AActor* FromThisRoom;

	FHeapElements(AActor* roomToVisit, float cost, AActor* fromThisRoom)
	{
		Cost = cost;
		RoomToVisit = roomToVisit;
		FromThisRoom = fromThisRoom;
	}

	FHeapElements()
	{
		Cost = 0;
		RoomToVisit = nullptr;
		FromThisRoom = nullptr;
	}

	bool operator<(const FHeapElements& Other) const
	{
		return Cost < Other.Cost;
	}
};

class TOONTANKS_API MST
{
public:
	MST();
	MST(std::vector<double> delaunaycoords, std::vector<std::size_t> delaunaytriangles,TMap<FVector2D, class AActor*> LookUpCorrdinatesToRoomTable);
	~MST();

	TArray<TTuple<AActor*, AActor*>> GetMSTFinalEdges() const;

protected:


private :
	void BuildEdgesMap();
	void PrintTriangles();

	void DebugPrintEdgesMap();

	void MinimumSpanTree();

	void ResetVisitedRooms();

	void PickRandomUnusedEdge();

	void DebugPrintMSTBuilt();
	void DebugPrintUnusedEdges();

	//Map of all the Edges between Rooms, key is a room and val is a set of its neighbors
	TMap<class AActor*, TSet<AActor*>> MapOfEdges;

	TMap<FVector2D, class AActor*> LookUpCorrdinatesToRoomTable;

	TSet<AActor*> VisitedRooms;
	TSet<AActor*> UnvisitedRooms;
	TSet<AActor*> RoomsInPQ;

	TArray<FHeapElements> PQHeap;

	std::vector<double> DelaunayCordinates;
	std::vector<std::size_t> DelaunayTriangles;

	TArray<FHeapElements> MSTBuilt;
	TArray<TTuple<AActor*, AActor*>> UnusedEdges;
	TArray<TTuple<AActor*, AActor*>> MSTFinalEdges;

	float PickUnusedEdgeProbability = 0.17f;

};

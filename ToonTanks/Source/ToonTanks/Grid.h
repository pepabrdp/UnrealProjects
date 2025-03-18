// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <map>
#include <vector>
#include <set>
#include <tuple>
#include "Grid.generated.h"

using namespace std;
class ARoomGenerator;
class AHallwayGenerator;


UCLASS()
class TOONTANKS_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector2D GetGridRowCol() const;

	void PlaceRoomAtCoordinate(int X, int Y);

	int GetOccupiedCell(int X, int Y) const;

	TMap<FVector2D, class AActor*> GetLookUpCorrdinatesToRoomTable() const;

	TArray<TTuple<AActor*, AActor*>> GetMSTFinalEdges() const;

	float GetCellSize() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private :

	
	void DelaunayTriangulation();

	TArray<double> ConstructDelaunayInputArray();

	void DebugPrintArray(TArray<double> Array);

	void DisplayMSTTree();

	UPROPERTY(VisibleAnywhere, Category = "Grid", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<int> GridArray;
	
	UPROPERTY(EditDefaultsOnly, Category = "Grid", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int LenX = 6;
	
	UPROPERTY(EditDefaultsOnly, Category = "Grid", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int LenY = 6;

	UFUNCTION(BlueprintCallable)
	void DisplayDebugGrid();

	UPROPERTY(EditDefaultsOnly, Category = "Grid", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CellSize = 150.f;


	UPROPERTY(EditDefaultsOnly, Category = "Room")
	TSubclassOf<ARoomGenerator> RoomGeneratorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Room")
	TSubclassOf<AActor> RoomClass;

	UPROPERTY(VisibleAnywhere, Category = "Room", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> RoomsPlacedArray;

	UPROPERTY(EditDefaultsOnly, Category = "Room")
	TSubclassOf<AHallwayGenerator> HallwayGeneratorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Hallway")
	TSubclassOf<AActor> HallwayClass;

	std::vector<double> DelaunayCordinates;
	std::vector<size_t> DelaunayTriangles;

	TArray<TTuple<AActor*, AActor*>> MSTFinalEdges;

};

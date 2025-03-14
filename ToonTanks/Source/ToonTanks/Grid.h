// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"


class ARoomGenerator;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private :

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

};

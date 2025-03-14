// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGenerator.generated.h"

class AGrid;

UCLASS()
class TOONTANKS_API ARoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomGenerator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetGrid(AGrid* grid);

	UFUNCTION()
	void Generate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Room", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int NumRooms = 10;

	UPROPERTY(VisibleAnywhere, Category = "Grid", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGrid* Grid;
};

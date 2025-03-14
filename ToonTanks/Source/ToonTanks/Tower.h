// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"


/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()



public : 
	ATower();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void CheckFireCondition();

	bool InFireRange();

	FTimerHandle FireRateTimeHandler;

	class ATank* Tank;
private :

	UPROPERTY(EditAnywhere)
	float TowerRange = 500;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

};

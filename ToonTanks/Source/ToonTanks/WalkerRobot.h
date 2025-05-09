// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "WalkerRobot.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AWalkerRobot : public ABasePawn
{
	GENERATED_BODY()

public : 
	AWalkerRobot();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

protected :
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float RobotRange = 500;

	class ATank* Tank;

	FTimerHandle FireRateTimeHandler;

	void CheckFireCondition();

	bool InFireRange();
	
	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

};

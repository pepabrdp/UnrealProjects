// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeToonTanks.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AGameModeToonTanks : public AGameModeBase
{
	GENERATED_BODY()

public :
	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame, FVector LastRobotLocation);

	UFUNCTION(BlueprintCallable)
	FName ReturnFromIndex(int Index);

	UFUNCTION(BlueprintCallable)
	int GetFirstRandomIndex();

	UFUNCTION(BlueprintCallable)
	int GetSecondRandomIndex(int FirstIndex);

	UFUNCTION(BlueprintCallable)
	int GetThirdRandomIndex(int FirstIndex, int SecondIndex);

	UFUNCTION(BlueprintCallable)
	bool BoonPicked(int ArrayIndex);

	UFUNCTION(BlueprintCallable)
	int UpdateBoonIndex(int BoonOptionIndex);

	UFUNCTION(BlueprintCallable)
	int32 ReturnBoonCost(FName BoonName);

private :

	int LastPickedIndex = -1;

	int GetRandomInd();

	class ATank* Tank;

	class AToonTanksPlayerController* ToonTanksPlayerController;
	
	UPROPERTY(EditDefaultsOnly)
	float SecondsToGameStart = 4.f;

	void HandleGameStart();

	int32 TargetEnemies = 0;

	int32 GetTargetTowersCount();

	int32 GetTargetWalkerCount();

	void CheckWonGame( FVector LastRobotLocation);

	UPROPERTY(EditAnywhere)
	class AAmbientSound* AmbientSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* EnvironmentMusic;

	UFUNCTION()
	void PlayEnviromentMusic();

	UPROPERTY(EditDefaultsOnly, Category = "Boons", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> BoonNames;

	UPROPERTY(EditDefaultsOnly, Category = "Boons", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, int32> BoonCosts;

	UPROPERTY(EditDefaultsOnly, Category = "Boons", meta = (AllowPrivateAccess = "true"))
	USoundBase* NotEnoughGearSound;

	UPROPERTY(EditDefaultsOnly, Category = "Boons", meta = (AllowPrivateAccess = "true"))
	USoundBase* PurchasedBoonSound;
	

	UPROPERTY()
	int NumBoonsPicked = 0;

	UFUNCTION()
	void ImplementBoon(FName BoonName);

	UPROPERTY(EditAnywhere, Category = "AmmoIncreaseBoon")
	int AmmoIncrease = 10;


	UPROPERTY(EditDefaultsOnly, Category = "Gear")
	TSubclassOf<class AActor> GearClass;

	
};

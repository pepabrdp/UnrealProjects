// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	class USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	class USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* HeadShotImpactSound;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float ShotRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float BaseShotDamage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float HeadShotModifier = 2.f;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float LegShotModifier = 0.5f;

	bool CheckHeadShot(FHitResult HitResult);
	bool CheckLegShot(FHitResult HitResult);
	bool FigureOutBodyPartHit(FHitResult HitResult, FName ParentBone);

	bool GunTrace(FHitResult& OutHitResult, FVector& ShotDirection, AController* OwnerController);

};

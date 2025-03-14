// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"


struct FInputActionValue;


UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	UStaticMeshComponent* GetTurretMesh();

	UStaticMeshComponent* GetBaseMesh();

protected:

	void RotateTurret(FVector LookAtVector);

	/** Called for movement input */
	void Shoot();

	void ShootWithDmgModifier(float DmgModifier);

	void HandleDestruction();

private:
	UPROPERTY(VisibleAnywhere, Category = "Base Pawns", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, Category = "Base Pawns", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Base Pawns", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, Category = "Base Pawns", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnProjectilePoint;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShake;

};

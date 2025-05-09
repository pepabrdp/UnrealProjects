// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	SpawnProjectilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Ball Spawn"));
	SpawnProjectilePoint->SetupAttachment(TurretMesh);


}

void ABasePawn::RotateTurret(FVector LookAtVector)
{
	
	FVector TargetLocation = LookAtVector - TurretMesh->GetComponentLocation();	
	FRotator AngleAdjustment = FRotator(0.f,TargetLocation.Rotation().Yaw,0.f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			AngleAdjustment,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			InterpSpeed			
		)
	);
}

void ABasePawn::Shoot()
{
	if (Controller != nullptr)
	{
		if(!ProjectileClass)
		{
			UE_LOG(LogTemp,Error,TEXT("ProjectileClass is missing! Please add a ProjectileClass to %s"),*GetName());
			return;
		}
		//DrawDebugSphere(GetWorld(),SpawnProjectilePoint->GetComponentLocation(),10,10,FColor::Cyan,false,-1.0f);
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,SpawnProjectilePoint->GetComponentLocation(),SpawnProjectilePoint->GetComponentRotation());
		Projectile->SetOwner(this);
	}	
}

void ABasePawn::ShootWithDmgModifier(float DmgModifier)
{
	if(Controller != nullptr)
	{
		if(!ProjectileClass)
		{
			UE_LOG(LogTemp,Error,TEXT("ProjectileClass is missing! Please add a ProjectileClass to %s"),*GetName());
			return;
		}
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,SpawnProjectilePoint->GetComponentLocation(),SpawnProjectilePoint->GetComponentRotation());
		Projectile->SetOwner(this);
		Projectile->DamageModifier(DmgModifier);
	}
}

void ABasePawn::HandleDestruction()
{
	//TO DO : VISUAL/AUDIO
	if(DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,DeathParticles,GetActorLocation(),GetActorRotation());
	}
	if(DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation());
	}
	if(DeathCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShake);
	}
}

UStaticMeshComponent* ABasePawn::GetTurretMesh()
{
	return TurretMesh;
}

UStaticMeshComponent* ABasePawn::GetBaseMesh()
{
	return BaseMesh;
}
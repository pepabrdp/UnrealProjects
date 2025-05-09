// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "BasePawn.h"




// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = Projectile;

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Move Component"));
	ProjectileMoveComp->InitialSpeed = 1300.0f;
	ProjectileMoveComp->MaxSpeed = 1300.0f;


	//Smoke Trail Creation
	SmokeTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke trail component"));
	SmokeTrail->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Projectile->OnComponentHit.AddDynamic(this,&AProjectile::OnHit);

	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,LaunchSound,GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning,TEXT("On HIT, just hit %s"),*(OtherActor->GetActorNameOrLabel()));
	auto MyOwner = GetOwner();
	if (MyOwner == nullptr) 
	{	
		Destroy();
		return;
	}
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	//Call Apply Damage function only if there is another actor, its not the projectile or its owner
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if(Cast<ABasePawn>(OtherActor))
		{
			//UE_LOG(LogTemp, Warning,TEXT("Generates Apply Damage Event"));
			UGameplayStatics::ApplyDamage(OtherActor,BaseDamage,MyOwnerInstigator, this,DamageTypeClass);
			if(HitParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this,HitParticles,GetActorLocation(),GetActorRotation());
			}
			if(HitCameraShake)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
			}
		}
		
		if(HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation(),1,0.5);
		}
		
	}
	Destroy();

	
}


void AProjectile::DamageModifier(float DmgModifier)
{
	BaseDamage += DmgModifier;
}
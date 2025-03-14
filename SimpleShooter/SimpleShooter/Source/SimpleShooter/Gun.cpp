// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Skeletal Mesh"));
	GunMesh->SetupAttachment(RootComponent);	


}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	if(MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,GunMesh,TEXT("MuzzleFlashSocket"));
	}
	if(MuzzleSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleSound,GunMesh,TEXT("MuzzleFlashSocket"));
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return;
	AController* OwnerController = OwnerPawn->GetController();
	if(!OwnerController) return;

	FHitResult OutHitResult;
	FVector ShotDirection;
	bool Hit = GunTrace(OutHitResult, ShotDirection,OwnerController);
	if(Hit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,OutHitResult.Location,ShotDirection.Rotation());

		float ShotDamage = BaseShotDamage;
		if(CheckHeadShot(OutHitResult))
		{
			ShotDamage *= HeadShotModifier;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),HeadShotImpactSound,OutHitResult.Location);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),ImpactSound,OutHitResult.Location);
			if(CheckLegShot(OutHitResult))
			{
				ShotDamage *= LegShotModifier;
			}
		}
		if(OutHitResult.GetActor())
		{
			//UE_LOG(LogTemp,Warning, TEXT("SHOT DAMAGE = %f"),ShotDamage);
			FPointDamageEvent DamageEvent(ShotDamage,OutHitResult,ShotDirection,nullptr);
			OutHitResult.GetActor()->TakeDamage(ShotDamage,DamageEvent,OwnerController,this);
		}
	}

}

bool AGun::CheckHeadShot(FHitResult HitResult)
{
	return FigureOutBodyPartHit(HitResult,TEXT("neck_02"));
}
	
	
bool AGun::CheckLegShot(FHitResult HitResult)
{
	bool leftLeg = FigureOutBodyPartHit(HitResult,TEXT("thigh_l"));
	bool rightLeg = FigureOutBodyPartHit(HitResult,TEXT("thigh_r"));
	return leftLeg || rightLeg;

}


bool AGun::FigureOutBodyPartHit(FHitResult HitResult, FName TargetParentBoneName)
{
	UE_LOG(LogTemp,Warning, TEXT("Component Name = %s"),*HitResult.GetComponent()->GetName());
	USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(HitResult.GetComponent());
    
	if (!SkeletalMesh)
    {
        // If the capsule was hit, get the owning actor and find the skeletal mesh
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            SkeletalMesh = HitActor->FindComponentByClass<USkeletalMeshComponent>();
        }
    }

    if (SkeletalMesh)
    {
        // Use the impact point to find the closest bone
        FVector ImpactPoint = HitResult.ImpactPoint;
        FName ClosestBoneName = SkeletalMesh->FindClosestBone(ImpactPoint);

        if (ClosestBoneName.IsNone())
        {
            return false;
        }

		//UE_LOG(LogTemp, Log, TEXT("Closest Bone is: %s"),*ClosestBoneName.ToString());
		// Get bone indices
        int32 HitBoneIndex = SkeletalMesh->GetBoneIndex(ClosestBoneName);
        int32 TargetParentBoneIndex = SkeletalMesh->GetBoneIndex(TargetParentBoneName);

        if (HitBoneIndex != INDEX_NONE && TargetParentBoneIndex != INDEX_NONE)
        {
            // Traverse up the hierarchy to check if the hit bone is a child of the parent bone
            int32 CurrentBoneIndex = HitBoneIndex;
            while (CurrentBoneIndex != INDEX_NONE)
            {
                if (CurrentBoneIndex == TargetParentBoneIndex)
                {
                    // Hit bone is a child of "neck_02"
                    //UE_LOG(LogTemp, Log, TEXT("Hit a child of %s: %s"),*TargetParentBoneName.ToString(), *ClosestBoneName.ToString());
                    
                    return true;
                }

                // Move up the hierarchy
                //CurrentBoneIndex = SkeletalMesh->GetParentBoneIndex(CurrentBoneIndex);
				ClosestBoneName = SkeletalMesh->GetParentBone(ClosestBoneName);
				CurrentBoneIndex = SkeletalMesh->GetBoneIndex(ClosestBoneName);
            }
        }
        
    }
	
	return false;

}

bool AGun::GunTrace(FHitResult& OutHitResult, FVector& ShotDirection,AController* OwnerController)
{

	FVector CameraLocation;
	FRotator CameraRotation;
	OwnerController->GetPlayerViewPoint(CameraLocation,CameraRotation);
	//DrawDebugCamera(GetWorld(),OutLocation,OutRotation,90,2,FColor::Red,true);
	
	//Set Out Parameter SHotDirection
	ShotDirection = -CameraRotation.Vector();

	// Perform a line trace from the camera into the world
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * ShotRange); // Adjust range as necessary
	FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		TraceStart,
		TraceEnd,
		ECollisionChannel::ECC_GameTraceChannel1,
		TraceParams
	);
}

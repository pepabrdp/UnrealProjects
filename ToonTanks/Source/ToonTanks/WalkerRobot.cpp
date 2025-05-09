// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkerRobot.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AWalkerRobot::AWalkerRobot()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWalkerRobot::BeginPlay()
{
	Super::BeginPlay();
	
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    GetWorldTimerManager().SetTimer(FireRateTimeHandler,this, &AWalkerRobot::CheckFireCondition, FireRate,true);

}

// Called every frame
void AWalkerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(InFireRange())
    {
        UE_LOG(LogTemp,Warning,TEXT("Tank->GetActorLocation(): %s"),*Tank->GetActorLocation().ToString());
        UE_LOG(LogTemp,Warning,TEXT("GetTurretMesh()->GetComponentLocation(): %s"),*GetTurretMesh()->GetComponentLocation().ToString());
        FVector TargetLocation = Tank->GetActorLocation() - GetTurretMesh()->GetComponentLocation();	
	    FRotator AngleAdjustment = FRotator(TargetLocation.Rotation().Pitch,TargetLocation.Rotation().Yaw,0.f);
        UE_LOG(LogTemp,Warning,TEXT("TargetLocation: %s"),*TargetLocation.ToString());
        UE_LOG(LogTemp,Warning,TEXT("AngleAdjustment: %s"),*AngleAdjustment.ToString());
        RotateTurret(Tank->GetActorLocation());        
    }
	

}

void AWalkerRobot::HandleDestruction()
{
	Super::HandleDestruction();
    Destroy();
}

void AWalkerRobot::CheckFireCondition()
{
    if(!Tank)
    {
        return;
    }

    if(InFireRange() && Tank->bAlive)
    {
        Shoot();
    }
}

bool AWalkerRobot::InFireRange()
{
    if(Tank)
    {
        float Dist = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Dist < RobotRange)
        {
            return true;
        }
    }
    return false;
}
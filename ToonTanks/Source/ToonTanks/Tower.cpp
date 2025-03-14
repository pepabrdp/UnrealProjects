// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


ATower::ATower()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    GetWorldTimerManager().SetTimer(FireRateTimeHandler,this, &ATower::CheckFireCondition, FireRate,true);

}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());        
    }
	

}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
    Destroy();
}


void ATower::CheckFireCondition()
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

bool ATower::InFireRange()
{
    if(Tank)
    {
        float Dist = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Dist < TowerRange)
        {
            return true;
        }
    }
    return false;
}
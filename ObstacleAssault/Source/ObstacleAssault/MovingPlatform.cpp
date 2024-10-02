// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartVector = GetActorLocation();
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);

	RotatePlatform(DeltaTime);
	
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	FVector CurrLocation = GetActorLocation();
	
	CurrLocation = CurrLocation + PlatformVelocity * DeltaTime;
	SetActorLocation(CurrLocation);

	TotalDistance = FVector::Dist(StartVector,CurrLocation);
	if( TotalDistance > ToMoveDistance)
	{
		ReverserMoveDirection();
	}
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	UE_LOG(LogTemp,Display,TEXT("Rotation happening"));
	AddActorLocalRotation(RotationVelocity * DeltaTime);

}

void AMovingPlatform::ReverserMoveDirection()
{
	FVector MoveDirection = PlatformVelocity.GetSafeNormal();
	StartVector = StartVector + MoveDirection * ToMoveDistance;
	SetActorLocation(StartVector);
	PlatformVelocity = PlatformVelocity * -1;
}
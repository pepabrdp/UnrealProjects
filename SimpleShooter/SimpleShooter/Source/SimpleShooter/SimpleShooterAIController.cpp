// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleShooterCharacter.h"


// Called when the game starts or when spawned
void ASimpleShooterAIController::BeginPlay()
{
	Super::BeginPlay();

    if(AIBehavior)
    {
        RunBehaviorTree(AIBehavior);

        // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

        // GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"),PlayerPawn->GetActorLocation());

        // GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"),PlayerPawn->GetActorLocation());

        APawn* MyPawn = GetPawn();
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),MyPawn->GetActorLocation());

        
    }
	
}


// Called every frame
void ASimpleShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASimpleShooterAIController::IsDead() const
{
    ASimpleShooterCharacter* AICharacter = Cast<ASimpleShooterCharacter>(GetPawn());
    if(AICharacter)
    {
        return AICharacter->IsDead();
    }
    return true;
}
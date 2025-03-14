// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "SimpleShooterAIController.h"


void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    //PawnKilled->GetLocalViewingPlayerController();
    if(PlayerController)
    {
        EndGame(false);
    }

    /*
    if(PawnKilled->IsPlayerControlled())
    {
        EndGame(false);
        //PlayerController->GameHasEnded(nullptr,false);
        
    }
    */

    for(ASimpleShooterAIController* AIController : TActorRange<ASimpleShooterAIController>(GetWorld()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Inside Loop for AI Controllers!"))
        if(!AIController->IsDead())
        {
            return;
        }
    }

    EndGame(true);

    //UE_LOG(LogTemp,Log, TEXT("A pawn was killed!"));
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(),bIsWinner);
    }

}
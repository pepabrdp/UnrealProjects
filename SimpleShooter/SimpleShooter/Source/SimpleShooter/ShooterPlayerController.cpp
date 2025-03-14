// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    Crosshair = CreateWidget(this,CrosshairClass);
    if(Crosshair)
    {
        Crosshair->AddToViewport();
    }


}


void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus,bIsWinner);

    Crosshair->RemoveFromViewport();   
    if(bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this,WinScreenClass);
        if(WinScreen)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this,LoseScreenClass);
        if(LoseScreen)
        {
            LoseScreen->AddToViewport();
        }

    }
    GetWorldTimerManager().SetTimer(RestartTimer,this,&APlayerController::RestartLevel,RestartDelay);

    //UE_LOG(LogTemp,Error,TEXT("Game Has Ended called!"));
}
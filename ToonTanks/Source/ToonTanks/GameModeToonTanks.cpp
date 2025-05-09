// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeToonTanks.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "WalkerRobot.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Called when the game starts or when spawned
void AGameModeToonTanks::BeginPlay()
{
	Super::BeginPlay();
	
    TargetEnemies = GetTargetTowersCount();
    TargetEnemies += GetTargetWalkerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));
    AmbientSound = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());
    if(AmbientSound && EnvironmentMusic)
    {
        AmbientSound->GetAudioComponent()->SetSound(EnvironmentMusic);
    }
    HandleGameStart();


    

}

void AGameModeToonTanks::HandleGameStart()
{
    StartGame();

    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;

        FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true
        );
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,InputDelegate,SecondsToGameStart,false);
        FTimerHandle PlayEnvironmentMusicHandle;
        GetWorldTimerManager().SetTimer(PlayEnvironmentMusicHandle,this,&AGameModeToonTanks::PlayEnviromentMusic,SecondsToGameStart+1,false);

        FTimerHandle SpawnHUDHandle;
        GetWorldTimerManager().SetTimer(SpawnHUDHandle,this,&AGameModeToonTanks::SpawnHUD,SecondsToGameStart+1,false);
    }

    


}


int32 AGameModeToonTanks::GetTargetTowersCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this,ATower::StaticClass(),Towers);
    return Towers.Num();
}

int32 AGameModeToonTanks::GetTargetWalkerCount()
{
    TArray<AActor*> Walkers;
    UGameplayStatics::GetAllActorsOfClass(this,AWalkerRobot::StaticClass(),Walkers);
    return Walkers.Num();
}

void AGameModeToonTanks::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank)
    {   
        Tank->HandleDestruction();
        if(Tank->HasSoul())
        {
            if (Tank->GetActiveSoulComponent())
            {
                Tank->GetActiveSoulComponent()->Deactivate();
                Tank->GetActiveSoulComponent()->DestroyComponent();
            }
            Tank->LostSoul();
            Tank->Revive();
        }
        else
        {
            if(ToonTanksPlayerController)
            {
                ToonTanksPlayerController->SetPlayerEnabledState(false);
            }

            bool LostGame = false;
            GameOver(LostGame, Tank->GetActorLocation());
        }
    }
    else
    {
        AActor* Gear = GetWorld()->SpawnActor<AActor>(GearClass,DeadActor->GetActorLocation(),DeadActor->GetActorRotation());
        if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
        {
            DestroyedTower->HandleDestruction();
            --TargetEnemies;
            CheckWonGame(DeadActor->GetActorLocation());
        
        }
        else if(AWalkerRobot* DestroyedWalker = Cast<AWalkerRobot>(DeadActor))
        {
            DestroyedWalker->HandleDestruction();
            --TargetEnemies;
            CheckWonGame(DeadActor->GetActorLocation());
            
        }
    }
    
}

void AGameModeToonTanks::CheckWonGame(FVector LastRobotLocation)
{
    if(TargetEnemies == 0)
    {
        bool WonGame = true;
        GameOver(WonGame,LastRobotLocation);
    }
}


void AGameModeToonTanks::PlayEnviromentMusic()
{
    if(AmbientSound)
    {
        AmbientSound->Play();
    }    
}

int AGameModeToonTanks::GetRandomInd()
{
    return FMath::RandRange(0,BoonNames.Num()-1 - NumBoonsPicked);
}

int AGameModeToonTanks::GetFirstRandomIndex()
{
    if(BoonNames.Num() - NumBoonsPicked  > 0)
    {
        return GetRandomInd();
    }

    return -1;
}

int AGameModeToonTanks::GetSecondRandomIndex(int FirstIndex)
{
    if(BoonNames.Num() - NumBoonsPicked > 1)
    {
        int SecondIndex = GetRandomInd();
        while(SecondIndex == FirstIndex)
        {
            SecondIndex = GetRandomInd();
        }
        return SecondIndex;
    }

    return -1;
}


int AGameModeToonTanks::GetThirdRandomIndex(int FirstIndex, int SecondIndex)
{
    if(BoonNames.Num()- NumBoonsPicked > 2)
    {
        int ThirdIndex = GetRandomInd();
        while(ThirdIndex == FirstIndex || ThirdIndex == SecondIndex)
        {
            ThirdIndex = GetRandomInd();
        }
        return ThirdIndex;
    }

    return -1;
}

FName AGameModeToonTanks::ReturnFromIndex(int Index)
{
    if(Index >= 0 && Index < BoonNames.Num())
    {
        return BoonNames[Index];
    }

    return "";
}

bool AGameModeToonTanks::BoonPicked(int ArrayIndex)
{
    UE_LOG(LogTemp,Display,TEXT("BoonPicked  called! "));
    if(ArrayIndex >= 0 && ArrayIndex < BoonNames.Num())
    {
        FName BoonPickedName = BoonNames[ArrayIndex];
        
        if(Tank->GetGear() < ReturnBoonCost(BoonPickedName))
        {
            UE_LOG(LogTemp,Display,TEXT("Not enough gear to pay for the boon! "));
            UGameplayStatics::PlaySound2D(this,NotEnoughGearSound);
            return false;
        }
        //Swap the picked boon with the last boon in the array before the other picked boons
        BoonNames[ArrayIndex] = BoonNames[BoonNames.Num()-1- NumBoonsPicked];
        BoonNames[BoonNames.Num()-1 - NumBoonsPicked] = BoonPickedName;
        //Increment the number of boons picked so that random only searches until the last boon available aka not picked yet
        ++NumBoonsPicked;
        LastPickedIndex = ArrayIndex;
        //Call Implement Boon function. 
        ImplementBoon(BoonPickedName);
        Tank->PayBoon(ReturnBoonCost(BoonPickedName));
        UGameplayStatics::PlaySound2D(this,PurchasedBoonSound);

        UE_LOG(LogTemp,Display,TEXT("ImplementBoon  called! "));
        return true;
    }
    return false;
}

void AGameModeToonTanks::ImplementBoon(FName BoonName)
{
    FString BoonNameStr = BoonName.ToString();
    if(BoonNameStr == "Dash")
    {
        UE_LOG(LogTemp,Display,TEXT("Dash Boon called! "));
        //Unlock Dash ability for the player
        ATank* Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(Player)
        {
            Player->UnlockDash();
        }
    }
    else if(BoonNameStr == "Soul")
    {
        //Grant Soul for the player
        ATank* Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(Player)
        {
            Player->UnlockSoul();
        }

    }
    else if(BoonNameStr == "Invincible")
    {
        //Grant Invincibility for the player
    }
    else if(BoonNameStr == "MoreAmmo")
    {
        UE_LOG(LogTemp,Display,TEXT("MoreAmmo Boon called! "));
        //Increase the ammo of the player
        ATank* Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(Player)
        {
            Player->UnlockIncreaseMaxAmmo(AmmoIncrease);
        }
    }
    else if(BoonNameStr == "MoreDmg")
    {
        UE_LOG(LogTemp,Display,TEXT("MoreDmg Boon called! "));
        //Increase the dmg of the player
        ATank* Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(Player)
        {
            Player->UnlockDmgModifier();
        }
    }
    else if(BoonNameStr == "MoreHP")
    {
        //Increase the HP of the player
        ATank* Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(Player)
        {
            Player->UnlockMoreHP();
        }    
    }
    else if(BoonNameStr == "PartyHat")
    {
        ATank* Player = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(Player)
        {
            Player->UnlockPartyHat();
        }        
    }
}

int32 AGameModeToonTanks::ReturnBoonCost(FName BoonName)
{
    if(BoonCosts.Contains(BoonName))
    {
        return BoonCosts[BoonName];
    }

    return -1;
}

//Send to widge info that the boon was picked and index updates may need to be done.
int AGameModeToonTanks::UpdateBoonIndex(int BoonOptionIndex)
{
    if(BoonOptionIndex == BoonNames.Num() - NumBoonsPicked)
    {
        return LastPickedIndex;
    }
    return -1;
}
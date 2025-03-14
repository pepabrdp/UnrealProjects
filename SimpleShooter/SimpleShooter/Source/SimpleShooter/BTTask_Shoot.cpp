// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "SimpleShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");

}


EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp,NodeMemory);
    
    if(!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }
    
    ASimpleShooterCharacter* AI_Character = Cast<ASimpleShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if(!AI_Character)
    {
        return EBTNodeResult::Failed;        
    }

    AI_Character->Shoot();
    return EBTNodeResult::Succeeded;
}
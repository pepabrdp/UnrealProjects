// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeToonTanks.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrHealth = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this,&UHealthComponent::DamageTaken);

	ToonTanksGameMode = Cast<AGameModeToonTanks>(UGameplayStatics::GetGameMode(this));
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if(Damage <= 0.f) return;


	
	CurrHealth = FMath::Clamp(CurrHealth - Damage,0,CurrHealth);;
	//UE_LOG(LogTemp, Warning, TEXT("OH SHIT GOT HIT! CurrHealth = %f"),CurrHealth);
	if(CurrHealth <= 0)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}
}

float UHealthComponent::GetHealthPercentage() const
{
	return CurrHealth / MaxHealth;
}

float UHealthComponent::GetCurrHealth() const
{
	return CurrHealth;
}


float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::IncreaseHealthAndMaxHealth(float HealthIncrease)
{
	MaxHealth += HealthIncrease;
	CurrHealth += HealthIncrease;
}

void UHealthComponent::Heal(float HealAmount)
{
	CurrHealth = FMath::Clamp(CurrHealth + HealAmount,CurrHealth,MaxHealth);
}
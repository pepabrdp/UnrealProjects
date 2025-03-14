// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private :

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	float CurrHealth;

	//Callback function for the OnTakeDamage Delegate 
	//MUST HAVE this arguments in order
	/*
	DamageActor : Who suffered the damage
	Damage : How much it was caused
	DamageType : Built in unreal type that has different presets such as fire, explosion, poison etc, can create custom one
	AController : Controller responsible for the damage, who posses the pawn like a player, if there is no one, it can be null
	DamageCauser : Actor that causes the damage like the projectile 
	*/
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);


	class AGameModeToonTanks* ToonTanksGameMode;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintPure)
	float GetCurrHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealthAndMaxHealth(float HealthIncrease);
		
};

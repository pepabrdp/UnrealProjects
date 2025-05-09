// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

class UInputAction;
class UNiagaraSystem;
class UNiagaraComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* InputMapping;

public : 
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();
	
	
	UFUNCTION(BlueprintCallable)
	void ResurrectingbAlive();

	UPROPERTY(VisibleAnywhere, Category = "Tank", BlueprintReadOnly)
	bool bAlive = true;

	APlayerController* GetPlayerControllerPointer();

	void UnlockDash();

	void UnlockDmgModifier();

	void UnlockIncreaseMaxAmmo(int AmmoIncrease);

	UFUNCTION(BlueprintImplementableEvent)
	void UnlockPartyHat();

	UFUNCTION(BlueprintImplementableEvent)
	void UnlockMoreHP();

	void UnlockSoul();

	void PickUpAmmo(int AmmoPickedUp);

	UFUNCTION(BlueprintImplementableEvent)
	void Heal(int32 HealAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void Revive();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo() const;

	UFUNCTION(BlueprintPure)
	bool HasSoul() const;

	void LostSoul();

	UFUNCTION(BlueprintPure)
	UNiagaraComponent* GetActiveSoulComponent() const;

	UFUNCTION(BlueprintPure)
	int32 GetGear() const;

	UFUNCTION(BlueprintCallable)
	void PickUpGear(int32 GearToAdd);

	UFUNCTION(BlueprintCallable)
	void PayBoon(int32 GearToPay);

	void PlaySoulEffect();

	void PlayDashEffect();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for looking input */
	void Dash(const FInputActionValue& Value);

	/** Called for movement input */
	void ShootInput(const FInputActionValue& Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private :

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 1;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1;

	UPROPERTY(VisibleAnywhere, Category = "Tank", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Tank", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	APlayerController* PlayerControllerPointer;



	//Dmg Modifier Boon Code
	bool bUnlockedDmgModifier = false;

	UPROPERTY(EditAnywhere,Category = "Dash")
    float DmgModifier = 25;


	//Bellow is only Dash Boon related code
	UPROPERTY()
	bool bCanDash = true;

	UPROPERTY()
	bool bUnlockedDash = false;

	bool bIsDashing = false;

	UPROPERTY(EditAnywhere,Category = "Dash")
	float DashCooldown = 5.f;

	UPROPERTY(EditAnywhere,Category = "Dash")
	float DashSpeed = 2000.f;

	FTimerHandle DashCooldownTimer;  // Timer handle
	FTimerHandle StopDashTimer;  // Timer handle
    
	void DashCooldownReset();  // Function to reset dash

	void StopDash();


	UPROPERTY(EditAnywhere,Category = "Dash")
    float DashTime = 0.3f;
    UPROPERTY(VisibleAnywhere, Category = "Dash", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector DashStart;
	UPROPERTY(VisibleAnywhere, Category = "Dash", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FVector DashEnd;
	UPROPERTY(VisibleAnywhere, Category = "Dash", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float DashProgress = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Dash", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int LastDirectionPressed = 1;

	UPROPERTY(EditAnywhere, Category = "Dash", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UNiagaraSystem* DashEffect;

	//Ammo Related Code
	UPROPERTY(EditAnywhere,Category = "Ammo")
	int32 MaxAmmo = 10;

	UPROPERTY(EditAnywhere,Category = "Ammo")
	int32 CurrentAmmo = 10;


	//Health & Max Health Increase Boon Code
	UPROPERTY(EditAnywhere,Category = "Health", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 HealthIncrease = 75;


	//Soul Related Boon Code
	bool bUnlockedSoul = false;

	UPROPERTY(EditAnywhere, Category = "Soul", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UNiagaraSystem* SoulEffect;
    // Active Soul Niagara component reference (to control the effect)
	UPROPERTY(VisibleAnywhere, Category = "Soul", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* ActiveSoulEffect; 

	UPROPERTY(VisibleAnywhere, Category = "Tank", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SoulEffectSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Soul", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator SoulEffectRotation = FRotator(0.f,90.f,0.f);

	UPROPERTY(EditAnywhere, Category = "Soul", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SoulEffectScale = 1.f;


	//Gear Related Code
	UPROPERTY(VisibleAnywhere, Category = "Gear", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Gear = 0;
};

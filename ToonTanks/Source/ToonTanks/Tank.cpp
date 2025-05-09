// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"



ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set Up Sring Arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
	//Set Up Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);


	SoulEffectSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Soul Effect Spawn Point"));
	SoulEffectSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	PlayerControllerPointer = Cast<APlayerController>(GetController());

	CurrentAmmo = MaxAmmo;
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp,Display,TEXT("Is ticking! "));

	if (PlayerControllerPointer)
	{
		//UE_LOG(LogTemp,Display,TEXT("There is a PlayerController "));
		/* code */
		FHitResult HitResult;
		PlayerControllerPointer->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);
		/*
		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			25,
			10,
			FColor::Red,
			false,
			-1.0f
		);
		*/
		RotateTurret(HitResult.ImpactPoint);
	}

	if(bIsDashing)
	{
		DashProgress += DeltaTime / DashTime;
        SetActorLocation(FMath::Lerp(DashStart, DashEnd, DashProgress),true);
	}
	

}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}




void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
    // Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATank::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ATank::ShootInput);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ATank::Dash);}
	/*
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	*/
}

void ATank::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	//UE_LOG(LogTemp,Display,TEXT("Move called! "));
	if (Controller != nullptr)
	{
		//UE_LOG(LogTemp,Display,TEXT("Movement Vector X :%lf"),MovementVector.X);
		//UE_LOG(LogTemp,Display,TEXT("Movement Vector Y :%lf"),MovementVector.Y);
		
		LastDirectionPressed = MovementVector.Y != 0? MovementVector.Y : LastDirectionPressed;
		
		FVector ToMoveVector(0.f);
		ToMoveVector.X = MovementVector.Y * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
		//ToMoveVector.Y = MovementVector.X;
		AddActorLocalOffset(ToMoveVector ,true);

		FRotator ToRotateVector(0.f);
		ToRotateVector.Yaw = MovementVector.X * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalRotation(ToRotateVector);
		
		/*
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();	
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Move Got Rototation and YawRotation!"));

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		UE_LOG(LogTemp,Display,TEXT("Movement Vector X :%lf"),MovementVector.X);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Move added movement Input!"));
		*/
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No CONTROLLER at Move Pressed!"));		
	}
}

void ATank::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	

	if (Controller != nullptr)
	{	
		//UE_LOG(LogTemp,Display,TEXT("LookAxis Vector X :%lf"),LookAxisVector.X);
		//UE_LOG(LogTemp,Display,TEXT("LookAxis Vector Y :%lf"),LookAxisVector.Y);
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Look added Yaw and Pitch Input!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No CONTROLLER at Look!"));		
	}
}

void ATank::ShootInput(const FInputActionValue& Value)
{
	if(CurrentAmmo <= 0)
	{
		return;
	}
	if (Controller != nullptr)
	{
		if(bUnlockedDmgModifier)
		{
			ShootWithDmgModifier(DmgModifier);
		}
		else
		{
			Shoot();
		}
		CurrentAmmo--;
	}	
}

APlayerController* ATank::GetPlayerControllerPointer()
{
	return PlayerControllerPointer;
}

void ATank::Dash(const FInputActionValue& Value)
{
	if(!bUnlockedDash)
	{
		return;
	}
	if(bCanDash && !bIsDashing)
	{
		bIsDashing = true;
		bCanDash = false;

		DashStart = GetActorLocation();
		DashEnd = DashStart + GetActorForwardVector() * DashSpeed * LastDirectionPressed;		
		DashProgress = 0.f;
		PlayDashEffect();
        GetWorldTimerManager().SetTimer(StopDashTimer, this, &ATank::StopDash,DashTime, false);
	}
}

void ATank::StopDash()
{
	bIsDashing = false;

    // Start cooldown before next dash
	GetWorldTimerManager().SetTimer(DashCooldownTimer, this, &ATank::DashCooldownReset,DashCooldown, false);
}

void ATank::DashCooldownReset()
{
	bCanDash = true;
}

void ATank::UnlockDash()
{
	bUnlockedDash = true;
}

void ATank::UnlockDmgModifier()
{
	bUnlockedDmgModifier = true;
}

void ATank::UnlockIncreaseMaxAmmo(int AmmoIncrease)
{
	UE_LOG(LogTemp,Display,TEXT("UnlockIncreaseMaxAmmo called! "));
	MaxAmmo += AmmoIncrease;
	CurrentAmmo += AmmoIncrease;
	UE_LOG(LogTemp,Display,TEXT("MaxAmmo : %d"),MaxAmmo);
	UE_LOG(LogTemp,Display,TEXT("CurrentAmmo : %d"),CurrentAmmo);
}

void ATank::PickUpAmmo(int AmmoPickedUp)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + AmmoPickedUp,CurrentAmmo,MaxAmmo);
}

int32 ATank::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 ATank::GetMaxAmmo() const
{
	return MaxAmmo;
}
void ATank::UnlockSoul() 
{
	//Give Soul Effect, this is a bool that prevents tank from dying
	//Also spawns a soul lile effect around player.
	PlaySoulEffect();
	bUnlockedSoul = true;
}

void ATank::PlaySoulEffect()
{
	if (SoulEffect)
    {
        // Spawn the Niagara effect component
		ActiveSoulEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SoulEffect,
			SoulEffectSpawnPoint,
			NAME_None,
			FVector(0,0,0),
			SoulEffectRotation,
			FVector(1, 1, 1) * SoulEffectScale,
			EAttachLocation::SnapToTarget,
			false,
			ENCPoolMethod::None
		);
    
        // if (SoulComp)
        // {
        //     // Enable looping
        //     //SoulComp->SetVariableBool(FName("Loop"), true);

        //     // Save reference if you need to stop it later
        //     ActiveSoulEffect = SoulComp;
        // }
    }
}

void ATank::PlayDashEffect()
{
	FRotator DashRotation = GetBaseMesh()->GetComponentRotation() + FRotator(0, 90, 0) * LastDirectionPressed;
	if (DashEffect)
    {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), // World context
			DashEffect, // Niagara system asset
			GetBaseMesh()->GetComponentLocation(), // Spawn at offset
			DashRotation, // Default rotation
			FVector(1, 1, 1), // Default scale
			true, // Auto destroy when done
			true // Auto activate
		);
	}
}

bool ATank::HasSoul() const
{
	return bUnlockedSoul;
}

void ATank::LostSoul()
{
	bUnlockedSoul = false;
}

UNiagaraComponent* ATank::GetActiveSoulComponent() const
{
	return ActiveSoulEffect;
}

void ATank::ResurrectingbAlive()
{
	bAlive = true;
}

int32 ATank::GetGear() const
{
	return Gear;
}

void ATank::PickUpGear(int32 GearToAdd)
{
	Gear += GearToAdd;
}

void ATank::PayBoon(int32 GearToPay)
{
	Gear = FMath::Clamp(Gear - GearToPay,0,Gear);
}
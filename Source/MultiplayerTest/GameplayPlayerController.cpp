// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPlayerController.h"

#include "EMovementStates.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MultiplayerTestGameModeBase.h"
#include "TheBossGameInstance.h"
#include "Actors/GameplayActor.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"

AGameplayPlayerController::AGameplayPlayerController() { }

void AGameplayPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayPlayerController, M_PossessedPawn);
}

void AGameplayPlayerController::BeginPlay()
{
	M_GameInstanceRef = Cast<UTheBossGameInstance>(GetGameInstance());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(M_MovementMappingContext, 0);
		Subsystem->AddMappingContext(M_CombatMappingContext, 1);
	}
	
	SetupInputComponent();
}

void AGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (M_PossessedPawn)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			M_InputSet = true;
			
			// Movement
			EnhancedInputComponent->BindAction(M_MovementInputAction, ETriggerEvent::Triggered, this, &AGameplayPlayerController::Move);
			EnhancedInputComponent->BindAction(M_LookInputAction, ETriggerEvent::Triggered, this, &AGameplayPlayerController::Look);

			// Jumping
			EnhancedInputComponent->BindAction(M_JumpInputAction, ETriggerEvent::Triggered, this, &AGameplayPlayerController::Jump);

			// Crouching
			EnhancedInputComponent->BindAction(M_CrouchInputAction, ETriggerEvent::Started, this, &AGameplayPlayerController::Crouch);
			EnhancedInputComponent->BindAction(M_CrouchInputAction, ETriggerEvent::Completed, this, &AGameplayPlayerController::Crouch);

			// Sprinting
			EnhancedInputComponent->BindAction(M_SprintInputAction, ETriggerEvent::Started, this, &AGameplayPlayerController::Run);
			EnhancedInputComponent->BindAction(M_SprintInputAction, ETriggerEvent::Completed, this, &AGameplayPlayerController::Run);

			// Shooting
			EnhancedInputComponent->BindAction(M_ShootInputAction, ETriggerEvent::Triggered, this, &AGameplayPlayerController::Shoot);
			EnhancedInputComponent->BindAction(M_ShootInputAction, ETriggerEvent::Completed, this, &AGameplayPlayerController::Shoot);
			
			// Aiming
			EnhancedInputComponent->BindAction(M_AimInputAction, ETriggerEvent::Started, this, &AGameplayPlayerController::Aim);
			EnhancedInputComponent->BindAction(M_AimInputAction, ETriggerEvent::Completed, this, &AGameplayPlayerController::Aim);

			// Reloading
			EnhancedInputComponent->BindAction(M_ReloadInputAction, ETriggerEvent::Started, this, &AGameplayPlayerController::Reload);
		}
	}
}

void AGameplayPlayerController::Move(const FInputActionValue& Value)
{
	M_MovementVector = Value.Get<FVector2d>();

	if (M_PossessedPawn)
	{
		M_PossessedPawn->SetPlayerMovementVector(M_MovementVector);
		M_PossessedPawn->AddMovementInput(M_PossessedPawn->GetActorForwardVector(), M_MovementVector.Y);
		M_PossessedPawn->AddMovementInput(M_PossessedPawn->GetActorRightVector(), M_MovementVector.X);
	}
}

void AGameplayPlayerController::Jump(const FInputActionValue& Value)
{
	if (M_PossessedPawn) M_PossessedPawn->TryJump();
}

void AGameplayPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();

	if (M_PossessedPawn && M_PossessedPawn->M_CurrentState != EMovementStates::Dead)
	{
		if (M_PossessedPawn->M_IsAiming && M_GameInstanceRef)
		{
			M_PossessedPawn->AddControllerYawInput(LookAxisVector.X * M_GameInstanceRef->ScopedMouseSensitivity);
			M_PossessedPawn->AddControllerPitchInput(LookAxisVector.Y * M_GameInstanceRef->ScopedMouseSensitivity);
		}
		else if (M_GameInstanceRef)
		{
			M_PossessedPawn->AddControllerYawInput(LookAxisVector.X * M_GameInstanceRef->MouseSensitivity);
			M_PossessedPawn->AddControllerPitchInput(LookAxisVector.Y * M_GameInstanceRef->MouseSensitivity);
		}
	}
}

void AGameplayPlayerController::Crouch(const FInputActionValue& Value)
{
	const bool isCrouching = Value.Get<bool>();
	if (M_PossessedPawn) M_PossessedPawn->SetCrouching(isCrouching);
}

void AGameplayPlayerController::Run(const FInputActionValue& Value)
{
	const bool isRunning = Value.Get<bool>();
	if (M_PossessedPawn) M_PossessedPawn->SetRunning(isRunning);
}

void AGameplayPlayerController::Shoot(const FInputActionValue& Value)
{
	const bool isShooting = Value.Get<bool>();
	if (M_PossessedPawn) M_PossessedPawn->SetShooting(isShooting);
}

void AGameplayPlayerController::Aim(const FInputActionValue& Value)
{
	const bool isAiming = Value.Get<bool>();
	if (M_PossessedPawn) M_PossessedPawn->SetAiming(isAiming);
}

void AGameplayPlayerController::Reload(const FInputActionValue& Value)
{
	if (M_PossessedPawn) M_PossessedPawn->Reload();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPlayerController.h"

#include "EMovementStates.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MultiplayerTestGameModeBase.h"
#include "TheBossGameInstance.h"
#include "Actors/GameplayActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
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
	Super::BeginPlay();
	
	M_GameInstanceRef = Cast<UTheBossGameInstance>(GetGameInstance());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(M_MovementMappingContext, 0);
		Subsystem->AddMappingContext(M_CombatMappingContext, 1);
		Subsystem->AddMappingContext(M_UIMappingContext, 2);
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
			EnhancedInputComponent->BindAction(M_MovementInputAction, ETriggerEvent::Triggered,
				this, &AGameplayPlayerController::Move);
			EnhancedInputComponent->BindAction(M_LookInputAction, ETriggerEvent::Triggered,
				this, &AGameplayPlayerController::Look);

			// Jumping
			EnhancedInputComponent->BindAction(M_JumpInputAction, ETriggerEvent::Triggered,
				this, &AGameplayPlayerController::Jump);

			// Crouching
			EnhancedInputComponent->BindAction(M_CrouchInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::Crouch);
			EnhancedInputComponent->BindAction(M_CrouchInputAction, ETriggerEvent::Completed,
				this, &AGameplayPlayerController::Crouch);

			// Sprinting
			EnhancedInputComponent->BindAction(M_SprintInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::Run);
			EnhancedInputComponent->BindAction(M_SprintInputAction, ETriggerEvent::Completed,
				this, &AGameplayPlayerController::Run);

			// Shooting
			EnhancedInputComponent->BindAction(M_ShootInputAction, ETriggerEvent::Triggered,
				this, &AGameplayPlayerController::Shoot);
			EnhancedInputComponent->BindAction(M_ShootInputAction, ETriggerEvent::Completed,
				this, &AGameplayPlayerController::Shoot);
			
			// Aiming
			EnhancedInputComponent->BindAction(M_AimInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::Aim);
			EnhancedInputComponent->BindAction(M_AimInputAction, ETriggerEvent::Completed,
				this, &AGameplayPlayerController::Aim);

			// Reloading
			EnhancedInputComponent->BindAction(M_ReloadInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::Reload);

			// Scoreboard
			EnhancedInputComponent->BindAction(M_ToggleScoreboardInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::ScoreboardEnable);
			EnhancedInputComponent->BindAction(M_ToggleScoreboardInputAction, ETriggerEvent::Completed,
				this, &AGameplayPlayerController::ScoreboardDisable);

			// Pause Menu
			EnhancedInputComponent->BindAction(M_TogglePauseMenuInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::TogglePauseMenu);

			// Chat Menu
			EnhancedInputComponent->BindAction(M_OpenChatMenuInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::EnableChatMenu);
			EnhancedInputComponent->BindAction(M_CloseChatMenuInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::DisableChatMenu);

			// Inventory Menu
			EnhancedInputComponent->BindAction(M_ToggleInventoryInputAction, ETriggerEvent::Started,
				this, &AGameplayPlayerController::ToggleInventoryMenu);
		}
	}
}

void AGameplayPlayerController::Move(const FInputActionValue& Value)
{
	M_MovementVector = Value.Get<FVector2d>();

	if (M_PossessedPawn && !M_IsPaused)
	{
		M_PossessedPawn->SetPlayerMovementVector(M_MovementVector);
		M_PossessedPawn->AddMovementInput(M_PossessedPawn->GetActorForwardVector(), M_MovementVector.Y);
		M_PossessedPawn->AddMovementInput(M_PossessedPawn->GetActorRightVector(), M_MovementVector.X);
	}
}

void AGameplayPlayerController::Jump(const FInputActionValue& Value)
{
	if (M_PossessedPawn && !M_IsPaused) M_PossessedPawn->TryJump();
}

void AGameplayPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();

	if (M_PossessedPawn && M_PossessedPawn->M_CurrentState != EMovementStates::Dead && !M_IsPaused)
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
	if (M_PossessedPawn && !M_IsPaused) M_PossessedPawn->SetCrouching(isCrouching);
}

void AGameplayPlayerController::Run(const FInputActionValue& Value)
{
	const bool isRunning = Value.Get<bool>();
	if (M_PossessedPawn && !M_IsPaused) M_PossessedPawn->SetRunning(isRunning);
}

void AGameplayPlayerController::Shoot(const FInputActionValue& Value)
{
	const bool isShooting = Value.Get<bool>();
	if (M_PossessedPawn && !M_IsPaused) M_PossessedPawn->SetShooting(isShooting);
}

void AGameplayPlayerController::Aim(const FInputActionValue& Value)
{
	const bool isAiming = Value.Get<bool>();
	if (M_PossessedPawn && !M_IsPaused) M_PossessedPawn->SetAiming(isAiming);
}

void AGameplayPlayerController::Reload(const FInputActionValue& Value)
{
	if (M_PossessedPawn && !M_IsPaused) M_PossessedPawn->Reload();
}

//* Scoreboard
void AGameplayPlayerController::ScoreboardEnable(const FInputActionValue& Value)
{
	if (!M_IsPaused)
	{
		if (M_CreatedWidget) M_CreatedWidget->RemoveFromParent();
		
		M_CreatedWidget = CreateWidget<UUserWidget>(this, M_ScoreboardWidget);
		if (M_CreatedWidget) M_CreatedWidget->AddToViewport();
	}
}

void AGameplayPlayerController::ScoreboardDisable(const FInputActionValue& Value)
{
	if (M_CreatedWidget && !M_IsPaused) M_CreatedWidget->RemoveFromParent();
}
//*

//* Pause
void AGameplayPlayerController::TogglePauseMenu(const FInputActionValue& Value)
{
	if (M_IsInventoryOpen && M_IsPaused) M_IsPaused = false;
	else { M_IsPaused = !M_IsPaused; }
	
	if (M_IsPaused) { ToggleInputModePause(M_PauseMenuWidget, true); }
	else { ToggleInputModePause(nullptr, false); }
}
//*

//* Chat
void AGameplayPlayerController::EnableChatMenu(const FInputActionValue& Value)
{
	if (M_IsPaused) return;

	OnChatOpenedEvent.Broadcast();
	ToggleInputModePause(M_ChatMenuWidget, true);
}

void AGameplayPlayerController::DisableChatMenu(const FInputActionValue& Value)
{
	ToggleInputModePause(nullptr, false);
}
//*

//* Inventory
void AGameplayPlayerController::ToggleInventoryMenu(const FInputActionValue& Value)
{
	if (M_IsPaused && !M_IsInventoryOpen) return;
	
	if (!M_IsInventoryOpen)
	{
		OnInventoryToggledEvent.Broadcast(true);
		ToggleInputMode(true);
	}
	else
	{
		OnInventoryToggledEvent.Broadcast(false);
		ToggleInputMode(false);
	}
	
	M_IsInventoryOpen = !M_IsInventoryOpen;
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY"));
}
//*

void AGameplayPlayerController::ToggleInputModePause(TSubclassOf<UUserWidget> WidgetToCreate, bool PauseGame)
{
	if (PauseGame && WidgetToCreate)
	{
		M_IsPaused = true;
		
		if (M_CreatedWidget) M_CreatedWidget->RemoveFromParent();
		
		M_CreatedWidget = CreateWidget<UUserWidget>(this, WidgetToCreate);
		if (M_CreatedWidget) M_CreatedWidget->AddToViewport();
		
		const FInputModeGameAndUI PauseInputMode;
		SetInputMode(PauseInputMode);
		bShowMouseCursor = true;
	}
	else
	{
		M_IsPaused = false;

		if (M_CreatedWidget) M_CreatedWidget->RemoveFromParent();

		OnInventoryToggledEvent.Broadcast(false);
		M_IsInventoryOpen = false;
		
		const FInputModeGameOnly GameInputMode;
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
}

void AGameplayPlayerController::ToggleInputMode(bool IsUIMode)
{
	if (IsUIMode)
	{
		M_IsPaused = true;
		const FInputModeGameAndUI PauseInputMode;
		SetInputMode(PauseInputMode);
		bShowMouseCursor = true;
	}
	else
	{
		M_IsPaused = false;
		const FInputModeGameOnly GameInputMode;
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
}

UUserWidget* AGameplayPlayerController::GetWidgetUnderCursor()
{
	FVector2D MousePosition;
	FHitResult HitResult;
	if (!GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		return nullptr;
	}
	
	GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, true, HitResult);

	// Check if the hit result has a valid actor and get the widget component
	if (HitResult.GetComponent())
	{
		UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitResult.GetComponent());
		if (WidgetComponent)
		{
			return Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
		}
	}

	return nullptr;
}
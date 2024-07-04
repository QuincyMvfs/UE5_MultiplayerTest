// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayPlayerController.generated.h"

class UHealthComponent;
class UTheBossGameInstance;
class UInputAction;
class UInputMappingContext;
class AGameplayActor;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChatOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryToggled, bool, InventoryToggledOn);

UCLASS()
class MULTIPLAYERTEST_API AGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

	// DELEGATES
	UPROPERTY(BlueprintAssignable)
	FOnChatOpened OnChatOpenedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryToggled OnInventoryToggledEvent;

	
	//* INPUT MAPPING CONTEXTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* M_MovementMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* M_CombatMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* M_UIMappingContext;
	//*
	
	//* INPUT ACTIONS
	// MOVEMENT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_MovementInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_CrouchInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_SprintInputAction;

	// WEAPON
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ShootInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_AimInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ReloadInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_InteractInputAction;
	
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ToggleScoreboardInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_TogglePauseMenuInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_OpenChatMenuInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_CloseChatMenuInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ToggleInventoryInputAction;
	//*
		
public:
	AGameplayPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetupInputComponent() override;

	// MOVEMENT
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Jump(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Crouch(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Run(const FInputActionValue& Value);

	// WEAPON
	UFUNCTION(BlueprintCallable)
	void Shoot(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Aim(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Reload(const FInputActionValue& Value);

	// UI
	UFUNCTION(BlueprintCallable)
	void ScoreboardEnable(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void ScoreboardDisable(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void EnableChatMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisableChatMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void ToggleInventoryMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void ToggleInputMode(bool IsUIMode);

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetWidgetUnderCursor();
	
public:
	FVector2d M_MovementVector;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	AGameplayActor* M_PossessedPawn;
	
	UTheBossGameInstance* M_GameInstanceRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool M_InputSet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> M_ScoreboardWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> M_PauseMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> M_ChatMenuWidget;

	UUserWidget* M_CreatedWidget;

	bool M_IsPaused = false;
	bool M_DenyInput = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool M_IsInventoryOpen = false;
	
public:
	UFUNCTION(BlueprintCallable)
	void ToggleInputModePause(TSubclassOf<UUserWidget> WidgetToCreate, bool PauseGame);
	

};


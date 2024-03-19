// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayPlayerController.generated.h"

class UTheBossGameInstance;
class UInputAction;
class UInputMappingContext;
class AGameplayActor;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChatOpened);

UCLASS()
class MULTIPLAYERTEST_API AGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

	// DELEGATES
	UPROPERTY(BlueprintAssignable)
	FOnChatOpened OnChatOpenedEvent;
	
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

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ToggleScoreboardInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_TogglePauseMenuInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_OpenChatMenuInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_CloseChatMenuInputAction;
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

private:
	void ToggleInputMode(TSubclassOf<UUserWidget> WidgetToCreate, bool PauseGame);
	
};


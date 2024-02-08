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
UCLASS()
class MULTIPLAYERTEST_API AGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* M_MovementMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* M_CombatMappingContext;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ShootInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_AimInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* M_ReloadInputAction;

public:
	AGameplayPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void SpawnPlayer();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnPlayer();
	bool Server_SpawnPlayer_Validate();
	void Server_SpawnPlayer_Implementation();
	
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Crouch(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Run(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Shoot(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Aim(const FInputActionValue& Value);

public:
	FVector2d M_MovementVector;
	AGameplayActor* M_PossessedPawn;
	UTheBossGameInstance* M_GameInstanceRef;
};

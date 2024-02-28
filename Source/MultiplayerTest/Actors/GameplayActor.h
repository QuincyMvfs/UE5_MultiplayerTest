// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayActor.generated.h"

class UWidgetComponent;
class UHealthComponent;
class UCameraZoomComponent;
class UBaseWeaponComponent;
enum class EMovementStates : uint8;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);

UCLASS()
class MULTIPLAYERTEST_API AGameplayActor : public ACharacter
{
	GENERATED_BODY()

	// Viewport Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* M_PlayerCapsuleComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* M_PlayerModelSKC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* M_WeaponModelSKC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* M_MuzzleLocationComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCharacterMovementComponent* M_PlayerMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* M_PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* M_CameraSpringArm;

	//* Actor Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBaseWeaponComponent* M_WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraZoomComponent* M_CameraZoomComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UHealthComponent* M_PlayerHealthComponent;
	
	//*
	
public:
	AGameplayActor();

protected:
	virtual void BeginPlay() override;

//// CUSTOM FUNCTIONS
public:
	UPROPERTY(BlueprintAssignable)
	FOnRespawn OnRespawnEvent;
	
	void SetPlayerMovementVector(FVector2d Value);

	// Sets animation variables in the players Animation Blueprint
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting, bool& bIsReloading, bool& bIsHit,
	float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState);

	// SETS REPLICATED VARIABLES
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//* CROUCHING | CLIENT + SERVER
	UFUNCTION()
	void SetCrouching(bool Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCrouching(bool Value);
	bool Server_SetCrouching_Validate(bool Value);
	void Server_SetCrouching_Implementation(bool Value);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_SetCrouching(bool Value);
	bool Multi_SetCrouching_Validate(bool Value);
	void Multi_SetCrouching_Implementation(bool Value);
	//*

	//* RUNNING | CLIENT + SERVER
	UFUNCTION()
	void SetRunning(bool Value);

	UFUNCTION()
	void SetSprintingTrue();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunning(bool Value);
	bool Server_SetRunning_Validate(bool Value);
	void Server_SetRunning_Implementation(bool Value);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_SetRunning(bool Value);
	bool Multi_SetRunning_Validate(bool Value);
	void Multi_SetRunning_Implementation(bool Value);

	//*

	//* SHOOTING | SERVER + CLIENT
	UFUNCTION()
	void SetShooting(bool Value);

	UFUNCTION(Server, Reliable)
	void Server_SetShooting(bool Value);
	void Server_SetShooting_Implementation(bool Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetShooting(bool Value);
	void Multi_SetShooting_Implementation(bool Value);

	//* AIMING | CLIENT + SERVER
	UFUNCTION()
	void SetAiming(bool Value);

	UFUNCTION()
	void OnRep_SetAiming();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool Value);
	bool Server_SetAiming_Validate(bool Value);
	void Server_SetAiming_Implementation(bool Value);
	//*

	UFUNCTION()
	void SetDead(AActor* Killed, AActor* Killer);

	UFUNCTION(Server, Reliable)
	void Server_SetDead(AActor* Killed, AActor* Killer);
	void Server_SetDead_Implementation(AActor* Killed, AActor* Killer);
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void Multi_SetDead(AActor* Killer);
	// void Multi_SetDead_Implementation(AActor* Killer);

	UFUNCTION()
	void Respawn();
	
	UFUNCTION()
	void Reload();
	
	UFUNCTION()
	void TryJump();

//// VARIABLES
public:
	//* EDITABLE VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_ReloadSpeed = 1.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_DelayTillSprinting = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_CrouchSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_SprintSpeed = 900.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float M_AimingWalkingSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	float M_RespawnDelay = 5.0f;
	//*
	
	UPROPERTY(ReplicatedUsing = OnRep_SetAiming, VisibleAnywhere, BlueprintReadOnly)
	bool M_IsAiming;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMovementStates M_CurrentState;
	
private:
	FVector2d m_movementVector;
	
	
	UPROPERTY(Replicated)
	float m_currentSpeed;
	
	UPROPERTY(Replicated)
	float m_currentVelocity;
	
	UPROPERTY(Replicated)
	bool m_isShooting;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayActor.generated.h"

class UCameraZoomComponent;
class UBaseWeaponComponent;
enum class EMovementStates : uint8;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

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
	UArrowComponent* M_PlayerArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCharacterMovementComponent* M_PlayerMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* M_PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* M_CameraSpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBaseWeaponComponent* M_WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraZoomComponent* M_CameraZoomComponent;
	
public:
	// Sets default values for this character's properties
	AGameplayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayerMovementVector(FVector2d Value);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting, bool& bisReloading,
	float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState);

	// SETS REPLICATED VARIABLES
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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
	
	UFUNCTION()
	void SetRunning(bool Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunning(bool Value);
	bool Server_SetRunning_Validate(bool Value);
	void Server_SetRunning_Implementation(bool Value);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_SetRunning(bool Value);
	bool Multi_SetRunning_Validate(bool Value);
	void Multi_SetRunning_Implementation(bool Value);

	UFUNCTION()
	void SetShooting(bool Value);

	UFUNCTION()
	void SetAiming(bool Value);

	UFUNCTION()
	void SetSprintingTrue();

	UFUNCTION()
	void TryJump();

	UFUNCTION()
	void Reload();
	
public:
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

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool M_IsAiming;
	
private:
	FVector2d m_movementVector;

	// Bools
	UPROPERTY(Replicated)
	EMovementStates m_currentState;

	// UFUNCTION()
	// void OnRep_Crouching();
	
	UPROPERTY(Replicated)
	float m_currentSpeed;
	UPROPERTY(Replicated)
	float m_currentVelocity;
	UPROPERTY(Replicated)
	bool m_isShooting;
};

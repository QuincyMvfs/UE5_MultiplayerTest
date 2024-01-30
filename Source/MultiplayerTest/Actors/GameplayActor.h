// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayActor.generated.h"

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
	USkeletalMeshComponent* M_WeaponSKC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UArrowComponent* M_PlayerArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCharacterMovementComponent* M_PlayerMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* M_PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* M_CameraSpringArm;
	
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
	void GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting,
	float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState);

	UFUNCTION()
	void SetCrouching(bool Value);

	UFUNCTION()
	void SetRunning(bool Value);

	UFUNCTION()
	void SetShooting(bool Value);

	UFUNCTION()
	void SetAiming(bool Value);

private:
	FVector2d m_movementVector;

	// Bools
	EMovementStates m_currentState;
	bool m_isAiming;
	bool m_isShooting;
};

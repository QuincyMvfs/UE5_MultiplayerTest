// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayActor.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MULTIPLAYERTEST_API AGameplayActor : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* M_PlayerCapsuleComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* M_PlayerModelSKC;

	UPROPERTY(EditAnywhere)
	UArrowComponent* M_PlayerArrowComponent;
	
	UPROPERTY(EditAnywhere)
	UCharacterMovementComponent* M_PlayerMovement;

	UPROPERTY(EditAnywhere)
	UCameraComponent* M_PlayerCamera;

	UPROPERTY(EditAnywhere)
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Skeletal Mesh Component

};

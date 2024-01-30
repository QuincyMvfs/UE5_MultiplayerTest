// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor.h"

#include "ActorFactories/ActorFactorySkeletalMesh.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MultiplayerTest/EMovementStates.h"

// Sets default values
AGameplayActor::AGameplayActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule + Mesh + Movement
	M_PlayerCapsuleComponent = GetCapsuleComponent();
	M_PlayerCapsuleComponent->InitCapsuleSize(55.0f, 96.0f);
	M_PlayerModelSKC = GetMesh();
	M_PlayerArrowComponent = GetArrowComponent();
	M_PlayerMovement = GetCharacterMovement();

	// Camera
	M_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	M_CameraSpringArm->SetupAttachment(M_PlayerCapsuleComponent);
	M_PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	M_PlayerCamera->SetupAttachment(M_CameraSpringArm);
	M_PlayerCamera-> bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void AGameplayActor::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void AGameplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameplayActor::SetPlayerMovementVector(FVector2d Value)
{
	m_movementVector = Value;
}

void AGameplayActor::GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting,
	float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState)
{
	bIsFalling = M_PlayerMovement->IsFalling();
	bIsAiming = m_isAiming;
	bIsShooting = m_isShooting;
	CurrentSpeed = GetVelocity().Size();
	CurrentVelocity = GetVelocity();
	
	
}

// CROUCHING
void AGameplayActor::SetCrouching(bool Value)
{
	if (Value)
	{
		m_currentState = EMovementStates::Crouching;
		UE_LOG(LogTemp, Warning, TEXT("Crouched"))
	}
	else
	{
		m_currentState = EMovementStates::Idle;
		UE_LOG(LogTemp, Warning, TEXT("Standing"))
	}
}

// RUNNING
void AGameplayActor::SetRunning(bool Value)
{
	if (Value)
	{
		m_currentState = EMovementStates::Running;
		
		UE_LOG(LogTemp, Warning, TEXT("Sprint"))
	}
	else
	{
		m_currentState = EMovementStates::Idle;

		UE_LOG(LogTemp, Warning, TEXT("Stop Sprinting"))
	}
}

// SHOOTING
void AGameplayActor::SetShooting(bool Value)
{
	if (Value)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shooting"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop Shooting"))
	}
}

// AIMING
void AGameplayActor::SetAiming(bool Value)
{
	if (Value)
	{
		UE_LOG(LogTemp, Warning, TEXT("Aiming"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Aiming"))
	}
}

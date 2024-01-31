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
#include "MultiplayerTest/Components/BaseWeaponComponent.h"

// Sets default values
AGameplayActor::AGameplayActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule + Mesh + Movement
	M_PlayerCapsuleComponent = GetCapsuleComponent();
	M_PlayerCapsuleComponent->InitCapsuleSize(55.0f, 96.0f);
	M_PlayerModelSKC = GetMesh();
	M_WeaponModelSKC = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSKC"));
	M_PlayerArrowComponent = GetArrowComponent();
	M_PlayerMovement = GetCharacterMovement();

	// Camera
	M_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	M_CameraSpringArm->SetupAttachment(M_PlayerCapsuleComponent);
	M_PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	M_PlayerCamera->SetupAttachment(M_CameraSpringArm);
	M_PlayerCamera-> bUsePawnControlRotation = true;
	
	M_WeaponComponent = CreateDefaultSubobject<UBaseWeaponComponent>(TEXT("Weapon"));
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

// FORCIBLY SETS THE PLAYERS MOVEMENT VECTOR
void AGameplayActor::SetPlayerMovementVector(FVector2d Value)
{
	m_movementVector = Value;
}

// GETS THE VARIABLES USED BY THE ABP
void AGameplayActor::GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting, bool& bisReloading,
	float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState)
{
	bIsFalling = M_PlayerMovement->IsFalling();
	bIsAiming = m_isAiming;
	bIsShooting = m_isShooting;
	bisReloading = m_isReloading;
	CurrentSpeed = GetVelocity().Size();
	CurrentVelocity = GetVelocity();
	CurrentState = m_currentState;
	
}

// CROUCHING
void AGameplayActor::SetCrouching(bool Value)
{
	if (m_currentState == EMovementStates::Running
		|| m_currentState == EMovementStates::Sprinting
		|| m_currentState == EMovementStates::Jumping)
	{
		// Do nothing
	}
	else if (Value)
	{
		m_currentState = EMovementStates::Crouching;
		M_PlayerMovement->MaxWalkSpeed = M_CrouchSpeed;
	}
	else if (!Value)
	{
		m_currentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
	}
}

// RUNNING
void AGameplayActor::SetRunning(bool Value)
{
	FTimerHandle SprintDelay;
	
	if (Value && !m_isAiming)
	{
		m_currentState = EMovementStates::Running;
		M_PlayerMovement->MaxWalkSpeed = M_RunSpeed;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SprintDelay, this, &AGameplayActor::SetSprintingTrue, M_DelayTillSprinting);
		}
	}
	else if (!Value && !m_isAiming)
	{
		m_currentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		SprintDelay.Invalidate();
	}
}

// SHOOTING
void AGameplayActor::SetShooting(bool Value)
{
	if (Value)
	{
		M_WeaponComponent->ShootWeapon(M_PlayerCamera, this);
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
		m_isAiming = true;
		M_PlayerMovement->MaxWalkSpeed = M_AimingWalkingSpeed;
	}
	else
	{
		m_isAiming = false;
		if (m_currentState != EMovementStates::Crouching){  M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
	}
}

// MAKES THE PLAYER BEGIN SPRINTING
void AGameplayActor::SetSprintingTrue()
{
	if (m_currentState == EMovementStates::Running && !m_isAiming)
	{
		m_currentState = EMovementStates::Sprinting;
		M_PlayerMovement->MaxWalkSpeed = M_SprintSpeed;
	}
}

// JUMP
void AGameplayActor::TryJump()
{
	if (!m_isAiming && m_currentState != EMovementStates::Jumping
		&& m_currentState != EMovementStates::Crouching)
	{
		ACharacter::Jump();
	}
}

void AGameplayActor::Reload()
{
	M_WeaponComponent->TryReload();
}

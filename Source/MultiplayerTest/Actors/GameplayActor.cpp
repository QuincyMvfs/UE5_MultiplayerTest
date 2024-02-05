// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor.h"

#include "ActorFactories/ActorFactorySkeletalMesh.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MultiplayerTest/EMovementStates.h"
#include "MultiplayerTest/Components/BaseWeaponComponent.h"
#include "MultiplayerTest/Components/CameraZoomComponent.h"
#include "MultiplayerTest/Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGameplayActor::AGameplayActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	M_PlayerCapsuleComponent = GetCapsuleComponent();
	M_PlayerCapsuleComponent->InitCapsuleSize(55.0f, 96.0f);

	// Skeletal Meshs
	M_PlayerModelSKC = GetMesh();
	M_WeaponModelSKC = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSKC"));
	M_WeaponModelSKC->SetupAttachment(M_PlayerModelSKC, "RightHand");
	M_MuzzleLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	M_MuzzleLocationComponent->SetupAttachment(M_WeaponModelSKC, "b_gun_muzzleflash");

	// ETC
	M_PlayerArrowComponent = GetArrowComponent();
	M_PlayerMovement = GetCharacterMovement();

	// Camera
	M_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	M_CameraSpringArm->SetupAttachment(M_PlayerCapsuleComponent);
	M_PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	M_PlayerCamera->SetupAttachment(M_CameraSpringArm);
	M_PlayerCamera-> bUsePawnControlRotation = true;

	// Custom Classes
	M_WeaponComponent = CreateDefaultSubobject<UBaseWeaponComponent>(TEXT("Weapon"));
	M_CameraZoomComponent = CreateDefaultSubobject<UCameraZoomComponent>(TEXT("CameraZoomComponent"));
	M_CameraZoomComponent->SetCameraComponent(M_PlayerCamera, M_CameraSpringArm);
	M_PlayerHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	M_HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	M_HealthBar->SetupAttachment(M_PlayerModelSKC, "HeadTop_End");
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
	bIsAiming = M_IsAiming;
	bIsShooting = m_isShooting;
	bisReloading = M_WeaponComponent->M_IsReloading;
	CurrentSpeed = GetVelocity().Size();
	CurrentVelocity = GetVelocity();
	CurrentState = m_currentState;
	
}

void AGameplayActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGameplayActor, M_IsAiming, COND_SkipOwner);
	DOREPLIFETIME(AGameplayActor, m_currentState);
	DOREPLIFETIME(AGameplayActor, m_currentSpeed);
	DOREPLIFETIME(AGameplayActor, m_currentVelocity);
	DOREPLIFETIME(AGameplayActor, m_isShooting);
}

// CROUCHING CLIENT + SERVER
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
		M_CameraZoomComponent->CrouchWithCamera(true);
	}
	else if (!Value)
	{
		m_currentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		M_CameraZoomComponent->CrouchWithCamera(false);
	}

	if (!HasAuthority())
	{
		Server_SetCrouching(Value);
		UE_LOG(LogTemp, Warning, TEXT("Client Crouch"))
	}
	else
	{
		Multi_SetCrouching(Value);
		UE_LOG(LogTemp, Warning, TEXT("Server Crouch"))
	}
}

bool AGameplayActor::Server_SetCrouching_Validate(bool Value)
{
	return true;
}

void AGameplayActor::Server_SetCrouching_Implementation(bool Value)
{
	Multi_SetCrouching_Implementation(Value);
}

bool AGameplayActor::Multi_SetCrouching_Validate(bool Value)
{
	return true;
}

void AGameplayActor::Multi_SetCrouching_Implementation(bool Value)
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
// END OF CROUCHING

// RUNNING CLIENT + SERVER
void AGameplayActor::SetRunning(bool Value)
{
	FTimerHandle SprintDelay;
	
	if (Value && !M_IsAiming)
	{
		m_currentState = EMovementStates::Running;
		M_PlayerMovement->MaxWalkSpeed = M_RunSpeed;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SprintDelay, this, &AGameplayActor::SetSprintingTrue, M_DelayTillSprinting);
		}
	}
	else if (!Value && !M_IsAiming)
	{
		m_currentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		SprintDelay.Invalidate();
	}

	
	if (!HasAuthority()) // Client
	{
		Server_SetRunning(Value);
	}
	else // Server
	{
		Multi_SetRunning(Value);
	}
}

bool AGameplayActor::Server_SetRunning_Validate(bool Value)
{
	return true;
}

void AGameplayActor::Server_SetRunning_Implementation(bool Value)
{
	Multi_SetRunning(Value);
}

bool AGameplayActor::Multi_SetRunning_Validate(bool Value)
{
	return true;
}

void AGameplayActor::Multi_SetRunning_Implementation(bool Value)
{
	FTimerHandle SprintDelay;
	
	if (Value && !M_IsAiming)
	{
		m_currentState = EMovementStates::Running;
		M_PlayerMovement->MaxWalkSpeed = M_RunSpeed;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SprintDelay, this, &AGameplayActor::SetSprintingTrue, M_DelayTillSprinting);
		}
	}
	else if (!Value && !M_IsAiming)
	{
		m_currentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		SprintDelay.Invalidate();
	}
}
// END OF RUNNING 

// SHOOTING
void AGameplayActor::SetShooting(bool Value)
{
	if (Value)
	{
		M_WeaponComponent->ShootWeapon(M_PlayerCamera, this, M_MuzzleLocationComponent->GetComponentLocation());
		if (!m_isShooting) m_isShooting = true;
	}
	else
	{
		m_isShooting = false;
	}
}

//* AIMING | CLIENT + SERVER
void AGameplayActor::SetAiming(bool Value)
{
	if (Value)
	{
		M_IsAiming = true;
		M_PlayerMovement->MaxWalkSpeed = M_AimingWalkingSpeed;
		M_CameraZoomComponent->ZoomCamera(true);
	}
	else
	{
		M_IsAiming = false;
		if (m_currentState != EMovementStates::Crouching) { M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
		M_CameraZoomComponent->ZoomCamera(false);
	}

	if (!HasAuthority()){ Server_SetAiming(Value); }
	else { OnRep_SetAiming(); }
}

bool AGameplayActor::Server_SetAiming_Validate(bool Value) { return true; }
void AGameplayActor::Server_SetAiming_Implementation(bool Value)
{
	if (Value)
	{
		M_IsAiming = true;
		M_PlayerMovement->MaxWalkSpeed = M_AimingWalkingSpeed;
	}
	else
	{
		M_IsAiming = false;
		if (m_currentState != EMovementStates::Crouching) { M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
	}
	
	OnRep_SetAiming();
}

void AGameplayActor::OnRep_SetAiming()
{
	if (M_IsAiming)
	{
		M_IsAiming = true;
		M_PlayerMovement->MaxWalkSpeed = M_AimingWalkingSpeed;
	}
	else
	{
		M_IsAiming = false;
		if (m_currentState != EMovementStates::Crouching) { M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
	}
}
//*

// MAKES THE PLAYER BEGIN SPRINTING
void AGameplayActor::SetSprintingTrue()
{
	if (m_currentState == EMovementStates::Running && !M_IsAiming)
	{
		m_currentState = EMovementStates::Sprinting;
		M_PlayerMovement->MaxWalkSpeed = M_SprintSpeed;
	}
}

// JUMP
void AGameplayActor::TryJump()
{
	if (!M_IsAiming && m_currentState != EMovementStates::Jumping
		&& m_currentState != EMovementStates::Crouching)
	{
		ACharacter::Jump();
	}
}

void AGameplayActor::Reload()
{
	M_WeaponComponent->TryReload();
}



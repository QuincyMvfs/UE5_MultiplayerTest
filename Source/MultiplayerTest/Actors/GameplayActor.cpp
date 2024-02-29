// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MultiplayerTest/EMovementStates.h"
#include "MultiplayerTest/GameplayPlayerController.h"
#include "MultiplayerTest/GameplayPlayerState.h"
#include "MultiplayerTest/MultiplayerTestGameModeBase.h"
#include "MultiplayerTest/Components/BaseWeaponComponent.h"
#include "MultiplayerTest/Components/CameraZoomComponent.h"
#include "MultiplayerTest/Components/HealthComponent.h"
#include "MultiplayerTest/Components/VFXReplicationComponent.h"
#include "Net/UnrealNetwork.h"

//* Sets default values
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
	M_PlayerMovement = GetCharacterMovement();

	// Camera
	M_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	M_CameraSpringArm->SetupAttachment(M_PlayerCapsuleComponent);
	M_CameraSpringArm->bUsePawnControlRotation = true;
	M_PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	M_PlayerCamera->SetupAttachment(M_CameraSpringArm);
	M_PlayerCamera-> bUsePawnControlRotation = true;

	// Custom Classes
	M_WeaponComponent = CreateDefaultSubobject<UBaseWeaponComponent>(TEXT("Weapon"));
	M_CameraZoomComponent = CreateDefaultSubobject<UCameraZoomComponent>(TEXT("CameraZoomComponent"));
	M_CameraZoomComponent->SetCameraComponent(M_PlayerCamera, M_CameraSpringArm);
	M_PlayerHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	M_PlayerHealthComponent->M_Team = ETeams::Humans;
	M_PlayerHealthComponent->OnKilledEvent.AddDynamic(this, &AGameplayActor::SetDead);
	M_VFXReplicationComponent = CreateDefaultSubobject<UVFXReplicationComponent>(TEXT("VFXReplicationComponent"));
}
//*

// Called when the game starts or when spawned
void AGameplayActor::BeginPlay()
{
	Super::BeginPlay();
}

// FORCIBLY SETS THE PLAYERS MOVEMENT VECTOR
void AGameplayActor::SetPlayerMovementVector(FVector2d Value) { m_movementVector = Value; }

// GETS THE VARIABLES USED BY THE ABP
void AGameplayActor::GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting, bool& bIsReloading,
	bool& bIsHit, float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState)
{
	bIsFalling = M_PlayerMovement->IsFalling();
	bIsAiming = M_IsAiming;
	bIsShooting = m_isShooting;
	bIsReloading = M_WeaponComponent->M_IsReloading;
	bIsHit = M_PlayerHealthComponent->M_IsHit;
	CurrentSpeed = GetVelocity().Size();
	CurrentVelocity = GetVelocity();
	CurrentState = M_CurrentState;
	
}

// REPLICATES ALL REPLICATED VARIABLES
void AGameplayActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGameplayActor, M_IsAiming, COND_SkipOwner);
	DOREPLIFETIME(AGameplayActor, M_CurrentState);
	DOREPLIFETIME(AGameplayActor, m_currentSpeed);
	DOREPLIFETIME(AGameplayActor, m_currentVelocity);
	DOREPLIFETIME(AGameplayActor, m_isShooting);
}

//* CROUCHING | CLIENT + SERVER
void AGameplayActor::SetCrouching(bool Value)
{
	if (M_CurrentState == EMovementStates::Dead) return;

	if (M_CurrentState == EMovementStates::Running
		|| M_CurrentState == EMovementStates::Sprinting
		|| M_CurrentState == EMovementStates::Jumping)
	{
		// Do nothing
	}
	else if (Value)
	{
		M_CurrentState = EMovementStates::Crouching;
		M_PlayerMovement->MaxWalkSpeed = M_CrouchSpeed;
		M_CameraZoomComponent->CrouchWithCamera(true);
	}
	else if (!Value)
	{
		M_CurrentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		M_CameraZoomComponent->CrouchWithCamera(false);
	}

	if (!HasAuthority()) { Server_SetCrouching(Value); }
	else { Multi_SetCrouching(Value); }
}

// SERVER
bool AGameplayActor::Server_SetCrouching_Validate(bool Value) { return true; }
void AGameplayActor::Server_SetCrouching_Implementation(bool Value)
{
	Multi_SetCrouching_Implementation(Value);
}

// MULTICAST
bool AGameplayActor::Multi_SetCrouching_Validate(bool Value) { return true; }
void AGameplayActor::Multi_SetCrouching_Implementation(bool Value)
{
	if (M_CurrentState == EMovementStates::Running
	|| M_CurrentState == EMovementStates::Sprinting
	|| M_CurrentState == EMovementStates::Jumping)
	{
		// Do nothing
	}
	else if (Value)
	{
		M_CurrentState = EMovementStates::Crouching;
		M_PlayerMovement->Crouch();
		M_PlayerMovement->MaxWalkSpeed = M_CrouchSpeed;
	}
	else if (!Value)
	{
		M_CurrentState = EMovementStates::Idle;
		M_PlayerMovement->UnCrouch();
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
	}
}
//*

//* RUNNING CLIENT + SERVER
void AGameplayActor::SetRunning(bool Value)
{
	if (M_CurrentState == EMovementStates::Dead) return;
	
	FTimerHandle SprintDelay;
	if (Value && !M_IsAiming)
	{
		M_CurrentState = EMovementStates::Running;
		M_PlayerMovement->MaxWalkSpeed = M_RunSpeed;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SprintDelay, this, &AGameplayActor::SetSprintingTrue, M_DelayTillSprinting);
		}
	}
	else if (!Value && !M_IsAiming)
	{
		M_CurrentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		SprintDelay.Invalidate();
	}
	
	if (!HasAuthority()) { Server_SetRunning(Value); }
	else { Multi_SetRunning(Value); }
}

// SERVER
bool AGameplayActor::Server_SetRunning_Validate(bool Value) { return true; }
void AGameplayActor::Server_SetRunning_Implementation(bool Value)
{
	Multi_SetRunning(Value);
}

// MULTICAST
bool AGameplayActor::Multi_SetRunning_Validate(bool Value) { return true; }
void AGameplayActor::Multi_SetRunning_Implementation(bool Value)
{
	FTimerHandle SprintDelay;
	
	if (Value && !M_IsAiming)
	{
		M_CurrentState = EMovementStates::Running;
		M_PlayerMovement->MaxWalkSpeed = M_RunSpeed;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SprintDelay, this, &AGameplayActor::SetSprintingTrue, M_DelayTillSprinting);
		}
	}
	else if (!Value && !M_IsAiming)
	{
		M_CurrentState = EMovementStates::Idle;
		M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed;
		SprintDelay.Invalidate();
	}
}
//*

//* SHOOTING | SERVER + CLIENT
void AGameplayActor::SetShooting(bool Value)
{
	if (M_CurrentState == EMovementStates::Dead) return;
	
	if (!HasAuthority()) { Server_SetShooting(Value); }
	else { Multi_SetShooting(Value); }
}

void AGameplayActor::Server_SetShooting_Implementation(bool Value)
{
	Multi_SetShooting(Value);
}

void AGameplayActor::Multi_SetShooting_Implementation(bool Value)
{
	if (Value && M_CurrentState != EMovementStates::Sprinting)
	{
		if (!M_WeaponComponent->M_IsReloading)
		{
			if (IsLocallyControlled())
			{
				M_WeaponComponent->ShootWeapon(M_PlayerCamera, this,
					M_MuzzleLocationComponent->GetComponentLocation());
			}
		}
		else { m_isShooting = false; }
		
		if (!m_isShooting) m_isShooting = true;
	}
	else if (!Value) { m_isShooting = false; }

	if (m_isShooting == false && M_CurrentState == EMovementStates::Running) { SetRunning(true); }
}
//*

//* AIMING | CLIENT + SERVER
void AGameplayActor::SetAiming(bool Value)
{
	if (M_CurrentState == EMovementStates::Dead) return;
	
	if (Value)
	{
		M_IsAiming = true;
		M_PlayerMovement->MaxWalkSpeed = M_AimingWalkingSpeed;
		M_CameraZoomComponent->ZoomCamera(true);
	}
	else
	{
		M_IsAiming = false;
		if (M_CurrentState != EMovementStates::Crouching) { M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
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
		if (M_CurrentState != EMovementStates::Crouching) { M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
	}
	
	OnRep_SetAiming();
}

void AGameplayActor::OnRep_SetAiming()
{
	if (M_CurrentState == EMovementStates::Dead) return;

	if (M_IsAiming)
	{
		M_IsAiming = true;
		M_PlayerMovement->MaxWalkSpeed = M_AimingWalkingSpeed;
	}
	else
	{
		M_IsAiming = false;
		if (M_CurrentState != EMovementStates::Crouching) { M_PlayerMovement->MaxWalkSpeed = M_WalkSpeed; }
	}
}
//*

// MAKES THE PLAYER BEGIN SPRINTING
void AGameplayActor::SetSprintingTrue()
{
	if (M_CurrentState == EMovementStates::Dead) return;

	if (M_CurrentState == EMovementStates::Running && !M_IsAiming && !m_isShooting)
	{
		M_CurrentState = EMovementStates::Sprinting;
		M_PlayerMovement->MaxWalkSpeed = M_SprintSpeed;
	}
}

// JUMP
void AGameplayActor::TryJump()
{
	if (M_CurrentState == EMovementStates::Dead) return;

	if (!M_IsAiming && M_CurrentState != EMovementStates::Jumping
		&& M_CurrentState != EMovementStates::Crouching)
	{
		ACharacter::Jump();
	}
}

void AGameplayActor::Reload()
{
	if (M_CurrentState == EMovementStates::Dead) return;

	M_WeaponComponent->TryReload();
}

void AGameplayActor::SetDead(AActor* Killed, AActor* Killer)
{
	if (M_CurrentState == EMovementStates::Dead) return;
	
	M_CurrentState = EMovementStates::Dead;
	M_PlayerCapsuleComponent->SetCollisionProfileName("DeadPlayer");
	M_CameraSpringArm->bDoCollisionTest = false;
	
	OnRespawnEvent.Broadcast();

	if (IsLocallyControlled())
	{
		Server_SetDead(Killed, Killer);
	}
	
	if (GetWorld())
	{
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimer, this, &AGameplayActor::Respawn, M_RespawnDelay);
	}
	
}

void AGameplayActor::Server_SetDead_Implementation(AActor* Killed, AActor* Killer)
{
	if (AGameplayPlayerState* PS = GetPlayerState<AGameplayPlayerState>())
	{
		PS->PlayerDied();
		if (APawn* KillerPawn = Cast<APawn>(Killer))
		{
			if (AGameplayPlayerState* KPS = KillerPawn->GetPlayerState<AGameplayPlayerState>())
			{
				KPS->PlayerGotKill();
			}
		}
	}
}

void AGameplayActor::Respawn()
{
	if (AMultiplayerTestGameModeBase* GM = GetWorld()->GetAuthGameMode<AMultiplayerTestGameModeBase>())
	{
		if (AGameplayPlayerController* PlayerController = GetController<AGameplayPlayerController>())
		{
			GM->RespawnPlayer(PlayerController);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
	FString::Printf(TEXT("PLAYER CONTROLLER NULL")));
		}
	}
}

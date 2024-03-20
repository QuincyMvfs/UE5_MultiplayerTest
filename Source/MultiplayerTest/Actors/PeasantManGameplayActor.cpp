// Fill out your copyright notice in the Description page of Project Settings.


#include "PeasantManGameplayActor.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MultiplayerTest/EMovementStates.h"
#include "MultiplayerTest/Components/HealthComponent.h"
#include "MultiplayerTest/Components/ShotgunComponent.h"

APeasantManGameplayActor::APeasantManGameplayActor() : AGameplayActor()
{
	M_ShotgunStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotgunMesh"));
	M_ShotgunStaticMeshComponent->SetupAttachment(M_PlayerModelSKC, "RightHand");
	M_MuzzleLocationComponent->SetupAttachment(M_ShotgunStaticMeshComponent);

	M_ShotgunComponent = CreateDefaultSubobject<UShotgunComponent>(TEXT("ShotgunComponent"));
}

void APeasantManGameplayActor::BeginPlay()
{
	Super::BeginPlay();
}

void APeasantManGameplayActor::Multi_SetShooting_Implementation(bool Value)
{
	// Is holding down shoot button
	if (Value && M_CurrentState != EMovementStates::Sprinting
		&& !m_isShooting && m_canShoot)
	{
		// Check if they are reloading
		if (!M_ShotgunComponent->M_IsReloading)
		{
			// Only do this if they are locally controlled
			if (IsLocallyControlled())
			{
				M_ShotgunComponent->ShootWeapon(M_PlayerCamera, this,
					M_MuzzleLocationComponent->GetComponentLocation());
			}

			m_isShooting = true;
			m_canShoot = false;
			
			FTimerManager& TimerManager = GetWorldTimerManager();
			FTimerHandle ShootTimer;
			FTimerHandle RecoilTimer;
			TimerManager.SetTimer(RecoilTimer, this,
				&APeasantManGameplayActor::RecoilTimer, 0.27f);
			TimerManager.SetTimer(ShootTimer, this,
				&APeasantManGameplayActor::ShootingTimer, M_ShotgunComponent->M_DelayBetweenShots);
		}
		else
		{
			m_isShooting = false;
			m_canShoot = true;
		}
	}

	if (m_isShooting == false && M_CurrentState == EMovementStates::Running) { SetRunning(true); }
}

void APeasantManGameplayActor::Reload()
{
	if (M_CurrentState == EMovementStates::Dead) return;

	M_ShotgunComponent->TryReload();
}

void APeasantManGameplayActor::GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting,
	bool& bIsReloading, bool& bIsHit, float& CurrentSpeed, FVector& CurrentVelocity, EMovementStates& CurrentState)
{
	bIsFalling = M_PlayerMovement->IsFalling();
	bIsAiming = M_IsAiming;
	bIsShooting = m_isShooting;
	bIsReloading = M_ShotgunComponent->M_IsReloading;
	bIsHit = M_PlayerHealthComponent->M_IsHit;
	CurrentSpeed = GetVelocity().Size();
	CurrentVelocity = GetVelocity();
	CurrentState = M_CurrentState;
}

void APeasantManGameplayActor::RecoilTimer()
{
	m_isShooting = false;
}

void APeasantManGameplayActor::ShootingTimer()
{
	m_canShoot = true;
}

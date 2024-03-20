// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void UShotgunComponent::ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	if (TryShootWeapon())
	{
		m_nextTimeToShoot = GetWorld()->GetTimeSeconds() + M_DelayBetweenShots;
		m_currentMagazine--;

		if (m_pawnOwner->IsLocallyControlled())
		{
			for (int i = 0; i < M_TotalPellets; i++)
			{
				m_rayEndPoint = CalculateInaccuracy(cameraComponent);
				m_muzzleLocation = muzzleLocation; 
				m_hitEndPoint = PerformRaycast(m_startPoint, m_rayEndPoint, shooter, false);
				
				SpawnBulletTracer(muzzleLocation, m_hitEndPoint, FRotator::ZeroRotator);
			}
		}

		Server_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
	}
}

void UShotgunComponent::Server_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter,
	FVector muzzleLocation)
{
	OnShootEvent.Broadcast();
	
	for (int i = 0; i < M_TotalPellets; i++)
	{
		m_rayEndPoint = CalculateInaccuracy(cameraComponent);
		m_muzzleLocation = muzzleLocation; 
		m_hitEndPoint = PerformRaycast(m_startPoint, m_rayEndPoint, shooter, true);
				
		SpawnBulletTracer(muzzleLocation, m_hitEndPoint, FRotator::ZeroRotator);
	}

	Multi_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
}

void UShotgunComponent::Multi_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter,
	FVector muzzleLocation)
{
	if (m_pawnOwner)
	{
		if (!m_pawnOwner->IsLocallyControlled())
		{
			for (int i = 0; i < M_TotalPellets; i++)
			{
				m_rayEndPoint = CalculateInaccuracy(cameraComponent);
				m_muzzleLocation = muzzleLocation; 
				m_hitEndPoint = PerformRaycast(m_startPoint, m_rayEndPoint, shooter, false);
				
				SpawnBulletTracer(muzzleLocation, m_hitEndPoint, FRotator::ZeroRotator);
			}
		}
	
		if (M_FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, M_FireSound, muzzleLocation);
		}	
	}
}

FVector UShotgunComponent::CalculateInaccuracy(const UCameraComponent* CameraComponent)
{
	m_startPoint = CameraComponent->GetComponentLocation();
	m_forwardVector = CameraComponent->GetForwardVector();
	float InaccuracyRight = FMath::RandRange(-M_RecoilSpread, M_RecoilSpread);
	float InaccuracyUp = FMath::RandRange(-M_RecoilSpread, M_RecoilSpread);
	float InaccuracyForward = FMath::RandRange(-M_RecoilSpread, M_RecoilSpread);

	m_forwardVector = m_forwardVector.RotateAngleAxis(InaccuracyRight, FVector::RightVector);
	m_forwardVector = m_forwardVector.RotateAngleAxis(InaccuracyUp, FVector::UpVector);
	m_forwardVector = m_forwardVector.RotateAngleAxis(InaccuracyForward, FVector::ForwardVector);
	m_forwardVector.Normalize();

	return m_startPoint + (m_forwardVector * m_rayLength);
}

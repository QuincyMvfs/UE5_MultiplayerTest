// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"

#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerTest/Actors/GrenadeProjectile.h"

// Sets default values for this component's properties
UBaseWeaponComponent::UBaseWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_currentMagazine = M_MaxMagazineCapacity;
}


// Called when the game starts
void UBaseWeaponComponent::BeginPlay() { Super::BeginPlay(); }


// Called every frame
void UBaseWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// CHECK ALL VARIABLES TO SEE IF PLAYER CAN SHOOT
bool UBaseWeaponComponent::TryShootWeapon()
{
	// TRUE = CAN SHOOT
	// FALSE = CANT SHOOT
	if (m_nextTimeToShoot <= GetWorld()->GetTimeSeconds()
		&& m_currentMagazine > 0)
	{
		return true;
	}

	if (m_currentMagazine <= 0) TryReload();
	return false;
}

// SHOOT
void UBaseWeaponComponent::ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter)
{
	if (TryShootWeapon())
	{
		m_nextTimeToShoot = GetWorld()->GetTimeSeconds() + M_DelayBetweenShots;
		m_currentMagazine--;

		const FVector startPoint = cameraComponent->GetComponentLocation();
		const FVector forwardVector = cameraComponent->GetForwardVector();
		const FVector endPoint = startPoint + (forwardVector * m_rayLength);
		PerformRaycast(startPoint, endPoint, shooter);
		FActorSpawnParameters spawnParams;
		AGrenadeProjectile* spawnedGrenade = GetWorld()->SpawnActor<AGrenadeProjectile>(M_GrenadeActor, startPoint, shooter->GetActorRotation(), spawnParams);

	}
}

void UBaseWeaponComponent::TryReload()
{
	if (m_currentMagazine < M_MaxMagazineCapacity && !M_IsReloading)
	{
		M_IsReloading = true;
		FTimerHandle ReloadTimer;
		GetWorld()->GetTimerManager().SetTimer(
			ReloadTimer, this, &UBaseWeaponComponent::Reload, M_ReloadDuration);
	}
	
}

void UBaseWeaponComponent::Reload()
{
	M_IsReloading = false;
	m_currentMagazine = M_MaxMagazineCapacity;
}

void UBaseWeaponComponent::PerformRaycast(FVector startPoint, FVector endPoint, AActor* shooter)
{
	FHitResult hitResult;
	if (bool hitObject = UKismetSystemLibrary::LineTraceSingle(
		this, startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hitResult,
		true,FColor::White,FColor::Red, 0.2f))
	{
		if (AActor* hitActor = hitResult.GetActor())
		{
			if (UHealthComponent* hitHealth = hitActor->FindComponentByClass<UHealthComponent>())
			{
				hitHealth->TakeDamage(M_Damage, shooter, hitActor);
				UE_LOG(LogTemp, Warning, TEXT("Hit Valid Target"));
			}
		}
	}
}


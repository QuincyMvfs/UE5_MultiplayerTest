// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"

#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerTest/GameplayPlayerState.h"
#include "MultiplayerTest/MultiplayerTestGameModeBase.h"
#include "MultiplayerTest/Actors/GameplayActor.h"
#include "MultiplayerTest/EnumClasses/EObjectTypes.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBaseWeaponComponent::UBaseWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_currentMagazine = M_MaxMagazineCapacity;
}


// Called when the game starts
void UBaseWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	m_currentMagazine = M_MaxMagazineCapacity;
	M_IsReloading = false;

	m_pawnOwner = Cast<APawn>(GetOwner());
}

void UBaseWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseWeaponComponent, M_IsReloading);
	DOREPLIFETIME(UBaseWeaponComponent, m_startPoint);
	DOREPLIFETIME(UBaseWeaponComponent, m_rayEndPoint);
	DOREPLIFETIME(UBaseWeaponComponent, m_hitEndPoint);
	DOREPLIFETIME(UBaseWeaponComponent, m_forwardVector);
	DOREPLIFETIME(UBaseWeaponComponent, m_muzzleLocation);
}

// CHECK ALL VARIABLES TO SEE IF PLAYER CAN SHOOT
bool UBaseWeaponComponent::TryShootWeapon()
{
	// TRUE = CAN SHOOT
	// FALSE = CANT SHOOT
	if (m_nextTimeToShoot <= GetWorld()->GetTimeSeconds()
		&& m_currentMagazine > 0 && !M_IsReloading)
	{
		return true; 
	}

	if (m_currentMagazine <= 0) TryReload();
	
	return false;
}

// SHOOT
void UBaseWeaponComponent::ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	if (TryShootWeapon())
	{
		m_nextTimeToShoot = GetWorld()->GetTimeSeconds() + M_DelayBetweenShots;
		m_currentMagazine--;

		if (m_pawnOwner->IsLocallyControlled())
		{
			m_startPoint = cameraComponent->GetComponentLocation();
			m_forwardVector = cameraComponent->GetForwardVector();
			m_rayEndPoint = m_startPoint + (m_forwardVector * m_rayLength);
			m_muzzleLocation = muzzleLocation; 
			m_hitEndPoint = PerformRaycast(m_startPoint, m_rayEndPoint, shooter, false);
			
			SpawnBulletTracer(muzzleLocation, m_hitEndPoint, FRotator::ZeroRotator);
		}

		Server_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
	}
}

// CLIENT SHOOT
void UBaseWeaponComponent::Server_OnShootWeapon_Implementation(UCameraComponent* cameraComponent,
	AActor* shooter, FVector muzzleLocation)
{
	// Check if the hit registered on the server
	OnShootEvent.Broadcast();
	m_startPoint = cameraComponent->GetComponentLocation();
	m_forwardVector = cameraComponent->GetForwardVector();
	m_rayEndPoint = m_startPoint + (m_forwardVector * m_rayLength);
	m_muzzleLocation = muzzleLocation; 
	m_hitEndPoint = PerformRaycast(m_startPoint, m_rayEndPoint, shooter, true);

	// Spawn tracers and play sound on all other clients
	Multi_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
}

// SERVER SHOOT
void UBaseWeaponComponent::Multi_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	if (m_pawnOwner)
	{
		if (!m_pawnOwner->IsLocallyControlled())
		{
			SpawnBulletTracer(m_muzzleLocation, m_hitEndPoint, FRotator::ZeroRotator);
		}
	
		if (M_FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, M_FireSound, muzzleLocation);
		}	
	}
}

FVector UBaseWeaponComponent::PerformRaycast(FVector startPoint, FVector endPoint, AActor* shooter, bool bDealDamage)
{
	EObjectTypes ObjectType = EObjectTypes::Concrete;
	FHitResult hitResult;
	if (bool hitObject = UKismetSystemLibrary::LineTraceSingle(
		this, startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		true, TArray<AActor*>(), EDrawDebugTrace::None, hitResult,
		true))
	{
		bool HasHitActor = false;
		bool IsHeadshot = false;
		
		if (AActor* hitActor = hitResult.GetActor())
		{
			if (UHealthComponent* hitHealth = hitActor->FindComponentByClass<UHealthComponent>())
			{
				if (bDealDamage) DealDamage(M_Damage, shooter, hitActor, hitHealth, hitResult.BoneName);
				HasHitActor = true;
				if (hitResult.BoneName == "Head") { IsHeadshot = true; }
			}
		}

		if (bDealDamage)
		{
			FVector SurfaceNormal = hitResult.ImpactNormal;
			FRotator SpawnRotation = SurfaceNormal.Rotation() + FRotator(180.0f, 180.0f, 180.0f);
			if (HasHitActor) ObjectType = EObjectTypes::Flesh;
			OnHitEvent.Broadcast(hitResult.ImpactPoint, SpawnRotation, ObjectType, IsHeadshot);
		}
		
		return hitResult.Location;
	}

	return endPoint;
}

void UBaseWeaponComponent::SpawnBulletTracer(FVector startPoint, FVector endPoint, FRotator rotation)
{
	if (M_BulletTracer)
	{
		UNiagaraComponent* SpawnedTracer = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), M_BulletTracer,
			startPoint, rotation, FVector::One(), true);
		SpawnedTracer->SetVectorParameter("BeamEnd", endPoint);
	}
}

void UBaseWeaponComponent::DealDamage(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone)
{
	if (!Instigator->HasAuthority()) { Server_DealDamage(Amount, Instigator, Victim, HitHealth, HitBone); }
	else { Multi_DealDamage(Amount, Instigator, Victim, HitHealth, HitBone); }
}

bool UBaseWeaponComponent::Server_DealDamage_Validate(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth, FName HitBone) { return true; }
void UBaseWeaponComponent::Server_DealDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth, FName HitBone)
{
	Multi_DealDamage(Amount, Instigator, Victim, HitHealth, HitBone);
}

bool UBaseWeaponComponent::Multi_DealDamage_Validate(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth, FName HitBone) { return true;}
void UBaseWeaponComponent::Multi_DealDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth, FName HitBone)
{
	HitHealth->TakeDamage(Amount, Instigator, Victim, HitBone);
}


void UBaseWeaponComponent::TryReload()
{
	if (m_currentMagazine < M_MaxMagazineCapacity && !M_IsReloading)
	{
		M_IsReloading = true;
		FTimerHandle ReloadTimer;
		GetWorld()->GetTimerManager().SetTimer(
			ReloadTimer, this, &UBaseWeaponComponent::Reload, M_ReloadDuration);
		
		if (!GetOwner()->HasAuthority()) { Server_TryReload(); }
		else { Multi_TryReload(); }
	}
}

bool UBaseWeaponComponent::Server_TryReload_Validate() { return true; }
void UBaseWeaponComponent::Server_TryReload_Implementation()
{
	Multi_TryReload();
}

bool UBaseWeaponComponent::Multi_TryReload_Validate() { return true; }
void UBaseWeaponComponent::Multi_TryReload_Implementation()
{
	M_IsReloading = true;
	FTimerHandle ReloadTimer;
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimer, this, &UBaseWeaponComponent::Reload, M_ReloadDuration);
}

void UBaseWeaponComponent::Reload()
{
	M_IsReloading = false;
	m_currentMagazine = M_MaxMagazineCapacity;

	if (m_pawnOwner->IsLocallyControlled())
	{
		m_nextTimeToShoot = GetWorld()->GetTimeSeconds() + 0.1f;
	}
}


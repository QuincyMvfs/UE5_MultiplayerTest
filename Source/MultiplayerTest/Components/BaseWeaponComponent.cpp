// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"

#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerTest/MultiplayerTestGameModeBase.h"
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

}

void UBaseWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseWeaponComponent, M_IsReloading);
	DOREPLIFETIME(UBaseWeaponComponent, m_startPoint);
	DOREPLIFETIME(UBaseWeaponComponent, m_endPoint);
	DOREPLIFETIME(UBaseWeaponComponent, m_forwardVector);
	DOREPLIFETIME(UBaseWeaponComponent, m_muzzleLocation);
}

// SHOOT
void UBaseWeaponComponent::ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	if (TryShootWeapon())
	{
		m_nextTimeToShoot = GetWorld()->GetTimeSeconds() + M_DelayBetweenShots;
		m_currentMagazine--;

		m_startPoint = cameraComponent->GetComponentLocation();
		m_forwardVector = cameraComponent->GetForwardVector();
		m_endPoint = m_startPoint + (m_forwardVector * m_rayLength);
		m_muzzleLocation = muzzleLocation;
		PerformRaycast(m_startPoint, m_endPoint, shooter);

		if (M_FireSound) { UGameplayStatics::PlaySoundAtLocation(this, M_FireSound, muzzleLocation); }
		
		if (!shooter->HasAuthority()) { Server_OnShootWeapon(cameraComponent, shooter, muzzleLocation); }
		else { Multi_OnShootWeapon(cameraComponent, shooter, muzzleLocation); }
	}
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

// CLIENT SHOOT
bool UBaseWeaponComponent::Server_OnShootWeapon_Validate(UCameraComponent* cameraComponent, AActor* shooter,
	FVector muzzleLocation) { return true; }
void UBaseWeaponComponent::Server_OnShootWeapon_Implementation(UCameraComponent* cameraComponent,
	AActor* shooter, FVector muzzleLocation)
{
	Multi_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
}

// SERVER SHOOT
bool UBaseWeaponComponent::Multi_OnShootWeapon_Validate(UCameraComponent* cameraComponent,
	AActor* shooter, FVector muzzleLocation) { return true; }
void UBaseWeaponComponent::Multi_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	APawn* PawnOwner = Cast<APawn>(shooter);
	if (!PawnOwner->IsLocallyControlled())
	{
		PerformRaycast(m_startPoint, m_endPoint, shooter);
		
		if (M_FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, M_FireSound, muzzleLocation);
		}
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
}


void UBaseWeaponComponent::PerformRaycast(FVector startPoint, FVector endPoint, AActor* shooter)
{
	FHitResult hitResult;
	if (bool hitObject = UKismetSystemLibrary::LineTraceSingle(
		this, startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		true, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hitResult,
		true,FColor::White,FColor::Red, 0.1f))
	{
		if (AActor* hitActor = hitResult.GetActor())
		{
			if (UHealthComponent* hitHealth = hitActor->FindComponentByClass<UHealthComponent>())
			{
				DealDamage(M_Damage, shooter, hitActor, hitHealth);
			}
		}
		SpawnBulletTracer(m_muzzleLocation, hitResult.Location, m_forwardVector.Rotation());
	}
	else { SpawnBulletTracer(m_muzzleLocation, endPoint, m_forwardVector.Rotation()); }
}

void UBaseWeaponComponent::DealDamage(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_DealDamage(Amount, Instigator, Victim, HitHealth);
	}
	else
	{
		Multi_DealDamage(Amount, Instigator, Victim, HitHealth);
	}
}

bool UBaseWeaponComponent::Server_DealDamage_Validate(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth) { return true; }
void UBaseWeaponComponent::Server_DealDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth)
{
	Multi_DealDamage(Amount, Instigator, Victim, HitHealth);
}

bool UBaseWeaponComponent::Multi_DealDamage_Validate(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth) { return true;}
void UBaseWeaponComponent::Multi_DealDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim,
	UHealthComponent* HitHealth)
{
	HitHealth->TakeDamage(Amount, Instigator, Victim);
	if (AMultiplayerTestGameModeBase* GM = GetWorld()->GetAuthGameMode<AMultiplayerTestGameModeBase>())
	{
		GM->PlayerHit();
	}
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"

#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerTest/Actors/GameplayActor.h"
#include "MultiplayerTest/Actors/GrenadeProjectile.h"
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

// Called every frame
void UBaseWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// SHOOT SERVER
bool UBaseWeaponComponent::Server_OnShootWeapon_Validate(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	return true;
}

void UBaseWeaponComponent::Server_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	Multi_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
}

// MULTICAST SHOOT
bool UBaseWeaponComponent::Multi_OnShootWeapon_Validate(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	return true;
}

void UBaseWeaponComponent::Multi_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
{
	APawn* PawnOwner = Cast<APawn>(shooter);
	if (!PawnOwner->IsLocallyControlled())
	{
		const FVector startPoint = cameraComponent->GetComponentLocation();
		const FVector forwardVector = cameraComponent->GetForwardVector();
		const FVector endPoint = startPoint + (forwardVector * m_rayLength);
		
		FActorSpawnParameters spawnParams;
		AGrenadeProjectile* spawnedGrenade = GetWorld()->SpawnActor<AGrenadeProjectile>(M_GrenadeActor, muzzleLocation, shooter->GetActorRotation(), spawnParams);
		
		if (M_FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, M_FireSound, muzzleLocation);
			UE_LOG(LogTemp, Warning, TEXT("MULTICAST SOUND"));
		}
		
	}
	
}

void UBaseWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseWeaponComponent, M_IsReloading);
}

// SHOOT
void UBaseWeaponComponent::ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation)
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
		AGrenadeProjectile* spawnedGrenade = GetWorld()->SpawnActor<AGrenadeProjectile>(M_GrenadeActor, muzzleLocation, shooter->GetActorRotation(), spawnParams);

		if (M_FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, M_FireSound, muzzleLocation);
		}
		
		// CLIENT
		if (!shooter->HasAuthority())
		{
			Server_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
		}
		// SERVER
		else
		{
			Multi_OnShootWeapon(cameraComponent, shooter, muzzleLocation);
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


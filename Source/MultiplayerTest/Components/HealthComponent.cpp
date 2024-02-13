// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_currentHealth = M_MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	m_currentHealth = M_MaxHealth;
	M_IsDead = false;
	M_IsHit = false;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, m_currentHealth);
	DOREPLIFETIME(UHealthComponent, M_IsHit);
	DOREPLIFETIME(UHealthComponent, M_IsDead);
}

void UHealthComponent::TakeDamage(float Amount, AActor* Instigator, AActor* Victim, FName HitBone)
{
	if (GetOwner()->HasAuthority()) { Multi_TakeDamage(Amount, Instigator, Victim, HitBone); }
}

bool UHealthComponent::Multi_TakeDamage_Validate(float Amount, AActor* Instigator, AActor* Victim, FName HitBone) { return true; }
void UHealthComponent::Multi_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim, FName HitBone)
{
	if (m_currentHealth > 0)
	{
		const float MultipliedAmount = GetMultipliedDamage(Amount, HitBone);
		m_currentHealth -= MultipliedAmount;
		m_currentHealth = FMath::Clamp(m_currentHealth, 0.0f, M_MaxHealth);
		OnDamagedEvent.Broadcast(m_currentHealth / M_MaxHealth);
		SetIsHit();
	}

	if (m_currentHealth <= 0)
	{
		OnKilledEvent.Broadcast(Instigator);
		M_IsDead = true;
	}
}

float UHealthComponent::GetMultipliedDamage(float BaseDamage, FName HitBone)
{
	if (M_BoneMultiplier.Contains(HitBone))
	{
		return BaseDamage * M_BoneMultiplier[HitBone];
	}

	return BaseDamage;
}

void UHealthComponent::SetIsHit()
{
	M_IsHit = true;
	FTimerHandle HitTimer;
	GetWorld()->GetTimerManager().SetTimer(
		HitTimer, this, &UHealthComponent::SetIsNotHit, M_HitDuration);
}

void UHealthComponent::SetIsNotHit()
{
	if (GetOwner()->HasAuthority()) { Multi_SetIsNotHit(); }
}

void UHealthComponent::Multi_SetIsNotHit_Implementation()
{
	M_IsHit = false;
	OnStunCompletedEvent.Broadcast();
}


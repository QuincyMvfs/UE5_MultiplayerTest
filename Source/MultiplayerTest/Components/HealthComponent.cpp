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

void UHealthComponent::TakeDamage(float Amount, AActor* Instigator, AActor* Victim)
{
	if (GetOwner()->HasAuthority()) { Multi_TakeDamage(Amount, Instigator, Victim); }
}

bool UHealthComponent::Multi_TakeDamage_Validate(float Amount, AActor* Instigator, AActor* Victim) { return true; }
void UHealthComponent::Multi_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim)
{
	if (m_currentHealth > 0)
	{
		m_currentHealth -= Amount;
		m_currentHealth = FMath::Clamp(m_currentHealth, 0.0f, M_MaxHealth);
		OnDamagedEvent.Broadcast(m_currentHealth / M_MaxHealth);
		SetIsHit();
	}
	else if (m_currentHealth <= 0)
	{
		OnKilledEvent.Broadcast(Instigator);
		M_IsDead = true;
	}
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
	M_IsHit = false;
}


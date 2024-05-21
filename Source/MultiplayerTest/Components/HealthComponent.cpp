// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "MultiplayerTest/GameplayPlayerState.h"
#include "MultiplayerTest/MultiplayerGameStateBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_currentHealth = M_MaxHealth;
	m_pawnOwner = Cast<APawn>(GetOwner());
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	m_gameStateRef = GetWorld()->GetGameState<AMultiplayerGameStateBase>();
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
	if (M_IsDead) return;
	
	if (m_currentHealth > 0)
	{
		if (UHealthComponent* InstigatorHealth = Instigator->GetComponentByClass<UHealthComponent>())
		{
			if (InstigatorHealth->M_Team == this->M_Team && !m_gameStateRef->M_FriendlyFire)
			{
				return;
			}
		}
		
		const float MultipliedAmount = GetMultipliedDamage(Amount, HitBone);
		m_currentHealth -= MultipliedAmount;
		m_currentHealth = FMath::Clamp(m_currentHealth, 0.0f, M_MaxHealth);

		if (m_pawnOwner && m_pawnOwner->IsLocallyControlled())
		{
			Server_SendDamageDealtValues(Instigator, MultipliedAmount);
		}
		
		OnDamagedEvent.Broadcast(m_currentHealth / M_MaxHealth, Victim);
		SetIsHit();

	}

	if (m_currentHealth <= 0)
	{
		OnKilledEvent.Broadcast(Victim, Instigator);
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

void UHealthComponent::Heal(float Amount, AActor* Instigator, AActor* Victim)
{
	if (GetOwner()->HasAuthority()) { Multi_Heal(Amount, Instigator, Victim); }
}

void UHealthComponent::Multi_Heal_Implementation(float Amount, AActor* Instigator, AActor* Victim)
{
	if (M_IsDead) return;
	
	if (m_currentHealth > 0)
	{
		m_currentHealth += Amount;
		m_currentHealth = FMath::Clamp(m_currentHealth, 0.0f, M_MaxHealth);
		
		OnDamagedEvent.Broadcast(m_currentHealth / M_MaxHealth, Victim);
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
	if (GetOwner()->HasAuthority()) { Multi_SetIsNotHit(); }
}

void UHealthComponent::Multi_SetIsNotHit_Implementation()
{
	M_IsHit = false;
	OnStunCompletedEvent.Broadcast();
}

void UHealthComponent::SendDamageDealtValues(AActor* Instigator, float Amount)
{
	if (AGameplayPlayerState* PS = m_pawnOwner->GetPlayerState<AGameplayPlayerState>())
	{
		PS->PlayerTookDamage(Amount);
	}
	
	if (APawn* EnemyPawn = Cast<APawn>(Instigator))
	{
		if (AGameplayPlayerState* EPS = EnemyPawn->GetPlayerState<AGameplayPlayerState>())
		{
			EPS->PlayerDealtDamage(Amount);
			//UE_LOG(LogTemp, Warning, TEXT("DAMAGE DEALT: %f"), EPS->M_DamageDealt)
		}
	}
	
}

void UHealthComponent::Server_SendDamageDealtValues_Implementation(AActor* Instigator, float Amount)
{
	if (AGameplayPlayerState* PS = m_pawnOwner->GetPlayerState<AGameplayPlayerState>())
	{
		PS->PlayerTookDamage(Amount);
	}

	if (APawn* EnemyPawn = Cast<APawn>(Instigator))
	{
		if (AGameplayPlayerState* EPS = EnemyPawn->GetPlayerState<AGameplayPlayerState>())
		{
			EPS->PlayerDealtDamage(Amount);
			//UE_LOG(LogTemp, Warning, TEXT("DAMAGE DEALT: %f"), EPS->M_DamageDealt)
		}
	}
}

float UHealthComponent::GetCurrentHealth()
{
	return m_currentHealth;
}


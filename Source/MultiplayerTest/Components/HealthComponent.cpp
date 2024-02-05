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
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, m_currentHealth);
}

void UHealthComponent::TakeDamage(float Amount, AActor* Instigator, AActor* Victim)
{
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("OWNER: %s"), *GetOwner()->GetName());
		Server_TakeDamage(Amount, Instigator, Victim);
	}
	else { Multi_TakeDamage(Amount, Instigator, Victim); }
}

bool UHealthComponent::Server_TakeDamage_Validate(float Amount, AActor* Instigator, AActor* Victim) { return true; }
void UHealthComponent::Server_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim)
{
	Multi_TakeDamage(Amount, Instigator, Victim);
}

bool UHealthComponent::Multi_TakeDamage_Validate(float Amount, AActor* Instigator, AActor* Victim) { return true; }
void UHealthComponent::Multi_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim)
{
	if (m_currentHealth > 0)
	{
		m_currentHealth -= Amount;
		m_currentHealth = FMath::Clamp(m_currentHealth, 0.0f, M_MaxHealth);
		UE_LOG(LogTemp, Warning, TEXT("DAMAGE TAKEN: %f"), Amount)
		UE_LOG(LogTemp, Warning, TEXT("NEW HEALTH: %f"), m_currentHealth)
	}
}


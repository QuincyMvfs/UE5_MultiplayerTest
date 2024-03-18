// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPlayerState.h"

#include "Net/UnrealNetwork.h"

AGameplayPlayerState::AGameplayPlayerState()
{
	M_TotalHits = 0;	
}

void AGameplayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayPlayerState, M_TotalHits);
	DOREPLIFETIME(AGameplayPlayerState, M_PlayerPawn);
	DOREPLIFETIME(AGameplayPlayerState, M_PlayerColor);
	DOREPLIFETIME(AGameplayPlayerState, M_PlayerName);
	DOREPLIFETIME(AGameplayPlayerState, M_Kills);
	DOREPLIFETIME(AGameplayPlayerState, M_DamageDealt);
	DOREPLIFETIME(AGameplayPlayerState, M_DamageTaken);
	DOREPLIFETIME(AGameplayPlayerState, M_Deaths);
}

void AGameplayPlayerState::PlayerGotKill()
{
	if (HasAuthority())
	{
		M_Kills++;
		//UE_LOG(LogTemp, Warning, TEXT("%s: Kill Scored"), *M_PlayerName.ToString())
	}
}

void AGameplayPlayerState::PlayerTookDamage(float Amount)
{
	if (HasAuthority())
	{
		M_DamageTaken = M_DamageTaken + Amount;
		//UE_LOG(LogTemp, Warning, TEXT("%s: Took %f Damage"), *M_PlayerName.ToString(), Amount);
	}
}

void AGameplayPlayerState::PlayerDealtDamage(float Amount)
{
	if (HasAuthority())
	{
		M_DamageDealt = M_DamageDealt + Amount;
		//UE_LOG(LogTemp, Warning, TEXT("%s: Dealt %f Damage"), *M_PlayerName.ToString(), Amount);
	}
}

void AGameplayPlayerState::PlayerDied()
{
	if (HasAuthority())
	{
		M_Deaths++;
		//UE_LOG(LogTemp, Warning, TEXT("%s: Died"), *M_PlayerName.ToString())
	}
}

void AGameplayPlayerState::GetPlayerStats(FName& Name, int& Kills, int& Deaths, float& DamageDealt,
	float& DamageTaken, FLinearColor& Color)
{
	Name = M_PlayerName;
	Kills = M_Kills;
	Deaths = M_Deaths;
	DamageDealt = M_DamageDealt;
	DamageTaken = M_DamageTaken;
	Color = M_PlayerColor;
}

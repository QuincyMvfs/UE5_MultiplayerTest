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
}

void AGameplayPlayerState::PlayerHit()
{
	if (HasAuthority())
	{
		M_TotalHits++;
		UE_LOG(LogTemp, Warning, TEXT("PLAYER HITS: %d"), M_TotalHits)
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameStateBase.h"

#include "GameplayPlayerState.h"
#include "Net/UnrealNetwork.h"

AMultiplayerGameStateBase::AMultiplayerGameStateBase()
{
	TotalHits = 0;
}

void AMultiplayerGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameStateBase, TotalHits);
	DOREPLIFETIME(AMultiplayerGameStateBase, JoinedPlayers);
}

void AMultiplayerGameStateBase::PlayerHit()
{
	if (HasAuthority()) { TotalHits++; }
}

void AMultiplayerGameStateBase::PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor)
{
	PlayerState->M_PlayerPawn = CreatedActor;
	OnPlayerCreatedEvent.Broadcast(PlayerState);
}

void AMultiplayerGameStateBase::PlayerJoined(AGameplayPlayerState* PlayerState)
{
	if (HasAuthority()) { Multi_PlayerJoined(PlayerState); }
	else { Server_PlayerJoined(PlayerState); }
}

void AMultiplayerGameStateBase::Server_PlayerJoined_Implementation(AGameplayPlayerState* PlayerState)
{
	Multi_PlayerJoined(PlayerState);
}

void AMultiplayerGameStateBase::Multi_PlayerJoined_Implementation(AGameplayPlayerState* PlayerState)
{
	if (!JoinedPlayers.Contains(PlayerState))
	{
		JoinedPlayers.Add(PlayerState);
	}
}

void AMultiplayerGameStateBase::PlayerLeft(AGameplayPlayerState* PlayerState)
{
	JoinedPlayers.Remove(PlayerState);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameStateBase.h"

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
	if (HasAuthority())
	{
		TotalHits++;
		UE_LOG(LogTemp, Warning, TEXT("HITS: %d"), TotalHits);
	}
}

void AMultiplayerGameStateBase::PlayerCreated(APlayerController* Controller, AActor* Actor)
{
	OnPlayerCreatedEvent.Broadcast(Controller, Actor);
}

void AMultiplayerGameStateBase::PlayerJoined(APlayerController* NewPlayer)
{
	JoinedPlayers.Add(NewPlayer);
}

void AMultiplayerGameStateBase::PlayerLeft(APlayerController* NewPlayer)
{
	JoinedPlayers.Remove(NewPlayer);
}

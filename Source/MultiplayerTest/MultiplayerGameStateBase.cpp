// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameStateBase.h"

#include "GameplayPlayerState.h"
#include "IndexTypes.h"
#include "MultiplayerTestGameModeBase.h"
#include "Net/UnrealNetwork.h"

AMultiplayerGameStateBase::AMultiplayerGameStateBase()
{
	TotalHits = 0;
	
	ConstructColours();
	ConstructNames();
}

void AMultiplayerGameStateBase::ConstructColours()
{
	constexpr FLinearColor CyanColor(0.0f, 1.0f, 1.0f);
	constexpr FLinearColor BlueColor(0.0f, 0.0f, 1.0f);
	constexpr FLinearColor YellowColor(1.0f, 1.0f, 0.0f);
	constexpr FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
	constexpr FLinearColor PurpleColor(1.0f, 0.0f, 1.0f);

	BaseColors.Add(CyanColor);
	BaseColors.Add(BlueColor);
	BaseColors.Add(YellowColor);
	BaseColors.Add(GreenColor);
	BaseColors.Add(PurpleColor);
	
	AvailableColors = BaseColors;
}

void AMultiplayerGameStateBase::ConstructNames()
{
	BaseNames.Add(TEXT("Amathophobia"));
	BaseNames.Add(TEXT("Hocuspocus"));
	BaseNames.Add(TEXT("GummifyCorbel"));
	BaseNames.Add(TEXT("ClockworkLingo"));
	BaseNames.Add(TEXT("Nothingarian"));

	AvailableNames = BaseNames;
	// https://www.coolgenerator.com/username-generator
}

void AMultiplayerGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameStateBase, TotalHits);
	DOREPLIFETIME(AMultiplayerGameStateBase, JoinedPlayers);
	DOREPLIFETIME(AMultiplayerGameStateBase, AvailableColors);
	DOREPLIFETIME(AMultiplayerGameStateBase, BaseColors);
}

void AMultiplayerGameStateBase::PlayerHit()
{
	if (HasAuthority()) { TotalHits++; }
}

void AMultiplayerGameStateBase::PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor)
{
	if (HasAuthority()) { Multi_PlayerCreated(PlayerState, CreatedActor); }
	else { Server_PlayerCreated(PlayerState, CreatedActor); }
}

void AMultiplayerGameStateBase::Server_PlayerCreated_Implementation(AGameplayPlayerState* PlayerState,
	AGameplayActor* CreatedActor)
{
	Multi_PlayerCreated(PlayerState, CreatedActor);
}

void AMultiplayerGameStateBase::Multi_PlayerCreated_Implementation(AGameplayPlayerState* PlayerState,
	AGameplayActor* CreatedActor)
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
		OnPlayerJoinedEvent.Broadcast(PlayerState);
		JoinedPlayers.Add(PlayerState);
	}
}

void AMultiplayerGameStateBase::PlayerLeft(AGameplayPlayerState* PlayerState)
{
	JoinedPlayers.Remove(PlayerState);
	PlayerState->M_PlayerPawn->Destroy();
	OnPlayerLeftEvent.Broadcast(PlayerState);
}

FLinearColor AMultiplayerGameStateBase::SelectColor()
{
	const int RandInt = FMath::RandRange(0, AvailableColors.Num() - 1);
	const FLinearColor SelectedColor = AvailableColors[RandInt];
	AvailableColors.RemoveAt(RandInt);
	
	return SelectedColor;
}

FName AMultiplayerGameStateBase::GetRandomName()
{
	const int RandInt = FMath::RandRange(0, AvailableNames.Num() - 1);
	const FName SelectedName = AvailableNames[RandInt];
	AvailableNames.RemoveAt(RandInt);

	return SelectedName;
}


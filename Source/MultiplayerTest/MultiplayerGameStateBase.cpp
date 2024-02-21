// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameStateBase.h"

#include "GameplayPlayerState.h"
#include "IndexTypes.h"
#include "Net/UnrealNetwork.h"

AMultiplayerGameStateBase::AMultiplayerGameStateBase()
{
	TotalHits = 0;
	
	const FLinearColor RedColor(1.0f, 0.0f, 0.0f);
	const FLinearColor BlueColor(0.0f, 0.0f, 1.0f);
	const FLinearColor YellowColor(1.0f, 1.0f, 0.0f);
	const FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
	const FLinearColor PurpleColor(0.0f, 1.0f, 1.0f);

	BaseColors.Add(RedColor);
	BaseColors.Add(BlueColor);
	BaseColors.Add(YellowColor);
	BaseColors.Add(GreenColor);
	BaseColors.Add(PurpleColor);
	
	AvailableColors = BaseColors;
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

// FLinearColor AMultiplayerGameStateBase::SelectColor()
// {
// 	if (HasAuthority())
// 	{
// 		const int RandInt = FMath::RandRange(0, AvailableColors.Num() - 1);
// 		UE_LOG(LogTemp, Warning, TEXT("Rand Range: %d"), RandInt)
// 		UE_LOG(LogTemp, Warning, TEXT("Array Length: %d"), AvailableColors.Num() - 1);
// 		const FLinearColor SelectedColor = AvailableColors[RandInt];
// 		AvailableColors.RemoveAt(RandInt);
// 		// RemoveColor(RandInt);
// 		
// 		return SelectedColor;
// 	}
//
// 	return AvailableColors[0];
// }
//
// void AMultiplayerGameStateBase::RemoveColor(int Index)
// {
// 	if (HasAuthority()) { Server_RemoveColor(Index); }
// }
//
// void AMultiplayerGameStateBase::Server_RemoveColor_Implementation(int Index)
// {
// 	AvailableColors.RemoveAt(Index);
// }


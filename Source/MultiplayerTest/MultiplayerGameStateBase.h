// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/GameplayActor.h"
#include "GameFramework/GameStateBase.h"
#include "Util/ColorConstants.h"
#include "MultiplayerGameStateBase.generated.h"

class AGameplayPlayerState;
class AGameplayActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCreated, AGameplayPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoined, AGameplayPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLeft, AGameplayPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMessageSent);

UCLASS()
class MULTIPLAYERTEST_API AMultiplayerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMultiplayerGameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//* DELEGATE EVENTS
	UPROPERTY(BlueprintAssignable)
	FOnPlayerCreated OnPlayerCreatedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerJoined OnPlayerJoinedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerLeft OnPlayerLeftEvent;

	UPROPERTY(BlueprintAssignable)
	FOnMessageSent OnMessageSentEvent;
	//*
	
	void PlayerHit();

	//* PLAYER PAWN CREATED
	void PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);

	UFUNCTION(Server, Unreliable)
	void Server_PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);
	void Server_PlayerCreated_Implementation(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);
	void Multi_PlayerCreated_Implementation(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);
	//*
	
	//* PLAYER JOINED
	void PlayerJoined(AGameplayPlayerState* PlayerState);

	UFUNCTION(Server, Unreliable)
	void Server_PlayerJoined(AGameplayPlayerState* PlayerState);
	void Server_PlayerJoined_Implementation(AGameplayPlayerState* PlayerState);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayerJoined(AGameplayPlayerState* PlayerState);
	void Multi_PlayerJoined_Implementation(AGameplayPlayerState* PlayerState);
	//*
	
	//* PLAYER LEFT
	void PlayerLeft(AGameplayPlayerState* PlayerState);

	UFUNCTION(Server, Reliable)
	void Server_PlayerLeft(AGameplayPlayerState* PlayerState);
	void Server_PlayerLeft_Implementation(AGameplayPlayerState* PlayerState);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayerLeft(AGameplayPlayerState* PlayerState);
	void Multi_PlayerLeft_Implementation(AGameplayPlayerState* PlayerState);
	//*
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FLinearColor SelectColor();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetRandomName();

	//* CHAT
	UFUNCTION(BlueprintCallable)
	void SendMessage(AGameplayPlayerState* PlayerState, const FString& Message);

	UFUNCTION(Server, Unreliable)
	void Server_SendMessage(AGameplayPlayerState* PlayerState, const FString& Message);
	void Server_SendMessage_Implementation(AGameplayPlayerState* PlayerState, const FString& Message);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_SendMessage(AGameplayPlayerState* PlayerState, const FString& Message);
	void Multi_SendMessage_Implementation(AGameplayPlayerState* PlayerState, const FString& Message);

	
public:
	UPROPERTY(Replicated)
	uint16 TotalHits;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<AGameplayPlayerState*> JoinedPlayers;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> AvailableColors;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> BaseColors;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FName> AvailableNames;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FName> BaseNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_FriendlyFire = true;

	//* CHAT INFO
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<AGameplayPlayerState*> ChatUsers;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FString> ChatMessages;
	//*
	
private:
	void ConstructColours();
	void ConstructNames();

	int M_MaxChatMessages = 150;
};

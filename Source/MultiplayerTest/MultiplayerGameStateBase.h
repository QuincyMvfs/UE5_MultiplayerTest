// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerCreated, APlayerController*, Controller, AActor*, SpawnedActor);

UCLASS()
class MULTIPLAYERTEST_API AMultiplayerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMultiplayerGameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerCreated OnPlayerCreatedEvent;
	
	void PlayerHit();

	void PlayerCreated(APlayerController* Controller, AActor* Actor);
	
	void PlayerJoined(APlayerController* NewPlayer);

	// UFUNCTION(Server, Unreliable)
	// void Server_PlayerJoined(APlayerController* NewPlayer);
	// void Server_PlayerJoined_Implementation(APlayerController* NewPlayer);
	//
	// UFUNCTION(NetMulticast, Unreliable)
	// void Multi_PlayerJoined(APlayerController* NewPlayer);
	// void Multi_PlayerJoined_Implementation(APlayerController* NewPlayer);

	void PlayerLeft(APlayerController* NewPlayer);

	UPROPERTY(Replicated)
	uint16 TotalHits;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<APlayerController*> JoinedPlayers;
};

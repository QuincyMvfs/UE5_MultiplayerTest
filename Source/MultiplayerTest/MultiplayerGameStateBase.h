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

	void PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);

	UFUNCTION(Server, Unreliable)
	void Server_PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);
	void Server_PlayerCreated_Implementation(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayerCreated(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);
	void Multi_PlayerCreated_Implementation(AGameplayPlayerState* PlayerState, AGameplayActor* CreatedActor);
	
	
	void PlayerJoined(AGameplayPlayerState* PlayerState);

	UFUNCTION(Server, Unreliable)
	void Server_PlayerJoined(AGameplayPlayerState* PlayerState);
	void Server_PlayerJoined_Implementation(AGameplayPlayerState* PlayerState);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayerJoined(AGameplayPlayerState* PlayerState);
	void Multi_PlayerJoined_Implementation(AGameplayPlayerState* PlayerState);

	void PlayerLeft(AGameplayPlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FLinearColor SelectColor();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetRandomName();
	
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
	bool M_FriendlyFire = false;

private:
	void ConstructColours();
	void ConstructNames();
};

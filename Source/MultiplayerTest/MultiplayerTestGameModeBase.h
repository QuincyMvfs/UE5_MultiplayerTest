// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerTestGameModeBase.generated.h"

class AGameplayActor;
class APlayerStart;
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPawnCreated, AController*, Controller, AActor*, SpawnedActor);

UCLASS()
class MULTIPLAYERTEST_API AMultiplayerTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerTestGameModeBase();

	UPROPERTY(BlueprintAssignable)
	FOnPawnCreated OnPawnCreatedEvent;
	
	void PlayerHit();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void FindPlayerStarts();
	virtual APlayerStart* GetPlayerSpawnPoint();
	virtual void RespawnPlayer(AController* PlayerToRespawn);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APlayerController*> M_Players;
	
	UPROPERTY(VisibleAnywhere)
	TArray<APlayerStart*> PlayerStarts;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> BP_PlayerActor;
};

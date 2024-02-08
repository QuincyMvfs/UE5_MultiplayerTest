// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerTestGameModeBase.generated.h"

class AGameplayActor;
class APlayerStart;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API AMultiplayerTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerTestGameModeBase();

	void PlayerHit();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void FindPlayerStarts();
	virtual APlayerStart* GetPlayerSpawnPoint();
	virtual void RespawnPlayer(APlayerController* PlayerToRespawn);

public:
	UPROPERTY(VisibleAnywhere)
	TArray<APlayerController*> M_Players;
	
	UPROPERTY(VisibleAnywhere)
	TArray<APlayerStart*> PlayerStarts;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> BP_PlayerActor;
};

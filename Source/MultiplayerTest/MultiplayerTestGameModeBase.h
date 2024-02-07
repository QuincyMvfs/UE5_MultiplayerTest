// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerTestGameModeBase.generated.h"

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

	UPROPERTY(VisibleAnywhere)
	TArray<APlayerController*> M_Players;
};

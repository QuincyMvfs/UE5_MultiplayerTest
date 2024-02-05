// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API AMultiplayerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMultiplayerGameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void PlayerHit();

	UPROPERTY(Replicated)
	uint16 TotalHits;
};

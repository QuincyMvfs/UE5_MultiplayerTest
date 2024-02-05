// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API AGameplayPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGameplayPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void PlayerHit();
	
	UPROPERTY(Replicated)
	uint16 M_TotalHits;

};

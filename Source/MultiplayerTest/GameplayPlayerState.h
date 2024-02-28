// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumClasses/ETeams.h"
#include "GameFramework/PlayerState.h"
#include "GameplayPlayerState.generated.h"

enum class ETeams : uint8;
class AGameplayActor;

UCLASS()
class MULTIPLAYERTEST_API AGameplayPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGameplayPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void PlayerGotKill();
	void PlayerTookDamage(float Amount);
	void PlayerDealtDamage(float Amount);
	void PlayerDied();

public:
	UPROPERTY(Replicated)
	uint16 M_TotalHits;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	AGameplayActor* M_PlayerPawn;

	// User Stats
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FLinearColor M_PlayerColor;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FName M_PlayerName;

	// Game Stats
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int M_Kills = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float M_DamageDealt = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float M_DamageTaken = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int M_Deaths = 0;

};

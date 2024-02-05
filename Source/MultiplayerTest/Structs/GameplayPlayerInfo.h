// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "GameplayPlayerInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MULTIPLAYERTEST_API FGameplayPlayerInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	int PlayerNum = 0;
	FString PlayerName = "Unknown";
	int PlayerKills = 0;
};

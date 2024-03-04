// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWave.generated.h"

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AActor>> WaveEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemiesAtOnce = 5;
};

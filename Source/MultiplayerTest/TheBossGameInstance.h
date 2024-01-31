// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TheBossGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UTheBossGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTheBossGameInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Settings")
	float MouseSensitivity = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Settings")
	float ScopedMouseSensitivity = 0.35f;
};

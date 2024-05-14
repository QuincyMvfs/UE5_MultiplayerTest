// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTest/EnumClasses/EItemTypes.h"
#include "FItemInfo.generated.h"

enum class EItemTypes : uint8;

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemTypes ItemType = EItemTypes::Interactable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0), meta = (ClampMax = 100))
	int Quantity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsStackable = true;
};


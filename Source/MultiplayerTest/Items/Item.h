// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTest/Structs/FItemInfo.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class AGameplayActor;
class UInventoryComponent;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class MULTIPLAYERTEST_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Items)
	FItemInfo ItemInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* OwningInventory;

	UPROPERTY(Transient)
	UWorld* World;
	
	virtual void Use(AGameplayActor* OwningCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(AGameplayActor* OwningCharacter);
};

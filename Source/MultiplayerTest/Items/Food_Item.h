// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Food_Item.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UFood_Item : public UItem
{
	GENERATED_BODY()

	float HealAmount = 25.0f;
	
	virtual void Use(AGameplayActor* OwningCharacter) override;

	UFUNCTION(Server, Unreliable)
	void Server_HealPlayer(UHealthComponent* PlayerHealth, AGameplayActor* OwningCharacter);
	void Server_HealPlayer_Implementation(UHealthComponent* PlayerHealth, AGameplayActor* OwningCharacter);
};

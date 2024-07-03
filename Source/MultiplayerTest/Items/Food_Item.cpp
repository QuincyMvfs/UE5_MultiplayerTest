// Fill out your copyright notice in the Description page of Project Settings.


#include "Food_Item.h"

#include "MultiplayerTest/Actors/GameplayActor.h"
#include "MultiplayerTest/Components/HealthComponent.h"
#include "MultiplayerTest/Components/InventoryComponent.h"

void UFood_Item::Use(AGameplayActor* OwningCharacter)
{
	Super::Use(OwningCharacter);

	if(UHealthComponent* PlayerHealth = OwningCharacter->GetComponentByClass<UHealthComponent>())
	{
		Server_HealPlayer(PlayerHealth, OwningCharacter);
	}
}

void UFood_Item::Server_HealPlayer_Implementation(UHealthComponent* PlayerHealth, AGameplayActor* OwningCharacter)
{
	if (PlayerHealth->GetCurrentHealth() < PlayerHealth->M_MaxHealth)
	{
		PlayerHealth->Heal(HealAmount, OwningCharacter, OwningCharacter);
		UE_LOG(LogTemp, Warning, TEXT("HEAL ME!!"));
		if (OwningInventory)
		{
			OwningInventory->RemoveItem(this, this->ItemIndex);
		}
	}
}

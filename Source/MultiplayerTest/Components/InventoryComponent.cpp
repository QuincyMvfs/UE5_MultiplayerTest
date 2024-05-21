// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "MultiplayerTest/Items/Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	M_Capacity = 20;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UItem* item : M_DefaultItems)
	{
		AddItem(item);
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AGameplayActor, M_CurrentState);

}

bool UInventoryComponent::AddItem(UItem* ItemToAdd)
{
	if (M_Items.Num() >= M_Capacity || !ItemToAdd) { return false; }
	
	ItemToAdd->OwningInventory = this;
	ItemToAdd->World = GetWorld();
	M_Items.Add(ItemToAdd);
	
	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventoryComponent::RemoveItem(UItem* ItemToRemove)
{
	if (!ItemToRemove) { return false; }
	
	ItemToRemove->OwningInventory = nullptr;
	ItemToRemove->World = nullptr;
	M_Items.RemoveSingle(ItemToRemove);
	
	OnInventoryUpdated.Broadcast();
	return true;
}



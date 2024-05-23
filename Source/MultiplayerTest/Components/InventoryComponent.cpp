// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "MultiplayerTest/Items/Item.h"
#include "Net/UnrealNetwork.h"

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
	DOREPLIFETIME(UInventoryComponent, M_Items);
	DOREPLIFETIME(UInventoryComponent, M_Capacity);

}

void UInventoryComponent::AddItem(UItem* ItemToAdd)
{
	Multi_AddItem(ItemToAdd);
}

void UInventoryComponent::Multi_AddItem_Implementation(UItem* ItemToAdd)
{
	if (M_Items.Num() >= M_Capacity || !ItemToAdd) { return; }

	if (ItemToAdd->ItemInfo.IsStackable)
	{
		if (!CanStack(ItemToAdd))
		{
			ItemToAdd->OwningInventory = this;
			ItemToAdd->World = GetWorld();
			M_Items.Add(ItemToAdd);
		}
	}
	else
	{
		ItemToAdd->OwningInventory = this;
		ItemToAdd->World = GetWorld();
		M_Items.Add(ItemToAdd);
	}
	
	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::CanStack(UItem* ItemToCheck)
{
	for (UItem* item : M_Items)
	{
		if (item->ItemInfo.ItemName.ToString() == ItemToCheck->ItemInfo.ItemName.ToString())
		{
			item->ItemInfo.Quantity += ItemToCheck->ItemInfo.Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Contains"));
			return true;
		}
	}

	return false;
}

void UInventoryComponent::RemoveItem(UItem* ItemToRemove)
{
	Multi_RemoveItem(ItemToRemove);
}

void UInventoryComponent::Multi_RemoveItem_Implementation(UItem* ItemToRemove)
{
	if (!ItemToRemove) { return; }
	
	ItemToRemove->OwningInventory = nullptr;
	ItemToRemove->World = nullptr;
	M_Items.RemoveSingle(ItemToRemove);
	
	OnInventoryUpdated.Broadcast();
}



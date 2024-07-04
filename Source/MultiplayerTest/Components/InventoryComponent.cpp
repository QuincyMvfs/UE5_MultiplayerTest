// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "MultiplayerTest/Items/Item.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	startingIndex = 0;
	while (startingIndex < M_Capacity)
	{
		CreateBaseItem(M_BlankItem.GetDefaultObject(), startingIndex);
		startingIndex++;

	}

	startingIndex = 0;
	for (UItem* item : M_DefaultItems)
	{
		AddItem(item, startingIndex);
		startingIndex++;
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, M_Items);
	DOREPLIFETIME(UInventoryComponent, M_Capacity);
	DOREPLIFETIME(UInventoryComponent, m_currentAmountOfItems);

}

// ADD ITEM
void UInventoryComponent::AddItem(UItem* ItemToAdd, int ItemIndex)
{
	Multi_AddItem(ItemToAdd, ItemIndex);
}

void UInventoryComponent::Multi_AddItem_Implementation(UItem* ItemToAdd, int ItemIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%d / %d | Large = %d"), m_currentAmountOfItems, M_Capacity, m_currentAmountOfItems <= M_Capacity);
	if (m_currentAmountOfItems >= M_Capacity && ItemToAdd->OwningInventory != this || !ItemToAdd) { return; }

	UE_LOG(LogTemp, Warning, TEXT("PASSED!!"));

	// If item is stackable, check if it can stack, if it cant, add it to the item array
	if (ItemToAdd->ItemInfo.IsStackable)
	{
		if (!CanStack(ItemToAdd))
		{
			ItemToAdd->OwningInventory = this;
			ItemToAdd->World = GetWorld();
			ItemToAdd->ItemIndex = ItemIndex;

			M_Items[ItemIndex] = ItemToAdd;
			m_currentAmountOfItems++;
			//M_Items.Add(ItemToAdd);
		}
	}
	else
	{
		ItemToAdd->OwningInventory = this;
		ItemToAdd->World = GetWorld();
		ItemToAdd->ItemIndex = ItemIndex;

		M_Items[ItemIndex] = ItemToAdd;
		m_currentAmountOfItems++;
		//M_Items.Add(ItemToAdd);
	}
	
	OnInventoryUpdated.Broadcast();
}
//

// CREATE BASE ITEM
void UInventoryComponent::CreateBaseItem(UItem* ItemToAdd, int ItemIndex)
{
	Multi_CreateBaseItem(ItemToAdd, ItemIndex);
}

void UInventoryComponent::Multi_CreateBaseItem_Implementation(UItem* ItemToAdd, int ItemIndex)
{
	ItemToAdd->ItemIndex = ItemIndex;
	ItemToAdd->OwningInventory = this;
	M_Items.Add(ItemToAdd);
}
//

// MOVE ITEM
void UInventoryComponent::MoveItem(UItem* ItemToAdd, int ItemIndex)
{
	Multi_MoveItem(ItemToAdd, ItemIndex);
}

void UInventoryComponent::Multi_MoveItem_Implementation(UItem* ItemToAdd, int ItemIndex)
{
	M_Items[ItemToAdd->ItemIndex] = M_BlankItem.GetDefaultObject();
	
	ItemToAdd->OwningInventory = this;
	ItemToAdd->ItemIndex = ItemIndex;
	M_Items[ItemIndex] = ItemToAdd;

	OnInventoryUpdated.Broadcast();
}
//

bool UInventoryComponent::CanStack(UItem* ItemToCheck)
{
	for (UItem* item : M_Items)
	{
		if (item->ItemInfo.ItemName.ToString() == ItemToCheck->ItemInfo.ItemName.ToString())
		{
			item->ItemInfo.Quantity += ItemToCheck->ItemInfo.Quantity;
			startingIndex--;
			return true;
		}
	}

	return false;
}

void UInventoryComponent::SwapItem(UItem* PreviousItem, int newIndex)
{
	Multi_SwapItem(PreviousItem, newIndex);
}

void UInventoryComponent::Multi_SwapItem_Implementation(UItem* PreviousItem, int newIndex)
{
	const int previousIndex = PreviousItem->ItemIndex;
	UItem* newItem = M_Items[newIndex];
	UInventoryComponent* previousInventory = M_Items[previousIndex]->OwningInventory;
	UInventoryComponent* newInventory = M_Items[newIndex]->OwningInventory;

	// Previous Item
	M_Items[previousIndex] = newItem;
	M_Items[previousIndex]->ItemIndex = previousIndex;
	M_Items[previousIndex]->OwningInventory = newInventory;

	// New Item
	M_Items[newIndex] = PreviousItem;
	M_Items[newIndex]->ItemIndex = newIndex;
	M_Items[newIndex]->OwningInventory = previousInventory;

	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::RemoveItem(UItem* ItemToRemove, int ItemIndex)
{
	Multi_RemoveItem(ItemToRemove, ItemIndex);
}

void UInventoryComponent::Multi_RemoveItem_Implementation(UItem* ItemToRemove, int ItemIndex)
{
	if (!ItemToRemove) { return; }

	m_currentAmountOfItems--;
	ItemToRemove->OwningInventory = this;
	ItemToRemove->World = GetWorld();
	M_Items[ItemToRemove->ItemIndex] = M_BlankItem.GetDefaultObject();
	
	OnInventoryUpdated.Broadcast();
}



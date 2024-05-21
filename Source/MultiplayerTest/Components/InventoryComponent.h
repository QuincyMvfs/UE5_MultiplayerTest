// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class UItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	void AddItem(UItem* ItemToAdd);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_AddItem(UItem* ItemToAdd);
	void Multi_AddItem_Implementation(UItem* ItemToAdd);
	
	void RemoveItem(UItem* ItemToRemove);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_RemoveItem(UItem* ItemToRemove);
	void Multi_RemoveItem_Implementation(UItem* ItemToRemove);

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UItem*> M_DefaultItems;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Inventory", Replicated)
	int32 M_Capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", Replicated)
	TArray<UItem*> M_Items;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	

		
};

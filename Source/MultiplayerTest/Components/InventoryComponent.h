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

	UFUNCTION(BlueprintCallable)
	void AddItem(UItem* ItemToAdd, int ItemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_AddItem(UItem* ItemToAdd, int ItemIndex);
	void Multi_AddItem_Implementation(UItem* ItemToAdd, int ItemIndex);

	void CreateBaseItem(UItem* ItemToAdd, int ItemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_CreateBaseItem(UItem* ItemToAdd, int ItemIndex);
	void Multi_CreateBaseItem_Implementation(UItem* ItemToAdd, int ItemIndex);

	UFUNCTION(BlueprintCallable)
	void MoveItem(UItem* ItemToAdd, int ItemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_MoveItem(UItem* ItemToAdd, int ItemIndex);
	void Multi_MoveItem_Implementation(UItem* ItemToAdd, int ItemIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItem* ItemToRemove, int ItemIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_RemoveItem(UItem* ItemToRemove, int ItemIndex);
	void Multi_RemoveItem_Implementation(UItem* ItemToRemove, int ItemIndex);

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UItem*> M_DefaultItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	TSubclassOf<UItem> M_BlankItem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Inventory", Replicated)
	int32 M_Capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int m_currentAmountOfItems = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", Replicated)
	TArray<UItem*> M_Items;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool CanStack(UItem* ItemToCheck);
	int startingIndex = 0;
};

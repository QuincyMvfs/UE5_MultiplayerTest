// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "ContainerComponent.generated.h"

class AGameplayActor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChestOpened, APlayerController*, PlayerController, AGameplayActor*, InteractingActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChestClosed, APlayerController*, PlayerController, AGameplayActor*, InteractingActor);

UCLASS()
class MULTIPLAYERTEST_API UContainerComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnChestOpened OnChestOpenedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnChestClosed OnChestClosedEvent;
	
	UFUNCTION(BlueprintCallable)
	void OpenChest(APlayerController* PlayerController, AGameplayActor* InteractingActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_OpenChest(APlayerController* PlayerController, AGameplayActor* InteractingActor);
	void Multi_OpenChest_Implementation(APlayerController* PlayerController, AGameplayActor* InteractingActor);
	
	UFUNCTION(BlueprintCallable)
	void CloseChest(APlayerController* PlayerController, AGameplayActor* InteractingActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_CloseChest(APlayerController* PlayerController, AGameplayActor* InteractingActor);
	void Multi_CloseChest_Implementation(APlayerController* PlayerController, AGameplayActor* InteractingActor);
};

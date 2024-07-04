// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerComponent.h"

void UContainerComponent::OpenChest(APlayerController* PlayerController, AGameplayActor* InteractingActor)
{
	Multi_OpenChest(PlayerController, InteractingActor);
}

void UContainerComponent::Multi_OpenChest_Implementation(APlayerController* PlayerController, AGameplayActor* InteractingActor)
{
	OnChestOpenedEvent.Broadcast(PlayerController, InteractingActor);
}

void UContainerComponent::CloseChest(APlayerController* PlayerController, AGameplayActor* InteractingActor)
{
	Multi_CloseChest(PlayerController, InteractingActor);
}

void UContainerComponent::Multi_CloseChest_Implementation(APlayerController* PlayerController,
	AGameplayActor* InteractingActor)
{
	OnChestClosedEvent.Broadcast(PlayerController, InteractingActor);
}

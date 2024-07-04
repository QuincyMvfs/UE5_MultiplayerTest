// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "MultiplayerTest/Actors/GameplayActor.h"

void IInteractable::Interact_Implementation(APlayerController* PlayerController, AGameplayActor* PlayerActor)
{
	Execute_Interact(PlayerActor->_getUObject(), PlayerController, PlayerActor);
}

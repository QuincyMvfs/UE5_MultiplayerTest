// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerTestGameModeBase.h"

#include "UObject/ConstructorHelpers.h"

AMultiplayerTestGameModeBase::AMultiplayerTestGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/BP_GameplayCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
}

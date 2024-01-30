// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerTestGameModeBase.h"

AMultiplayerTestGameModeBase::AMultiplayerTestGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/BP_GameplayCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
}

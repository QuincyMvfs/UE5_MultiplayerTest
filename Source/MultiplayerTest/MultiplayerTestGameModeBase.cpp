// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerTestGameModeBase.h"

#include "MultiplayerGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerTestGameModeBase::AMultiplayerTestGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/BP_GameplayCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
}

void AMultiplayerTestGameModeBase::PlayerHit()
{
	if (AMultiplayerGameStateBase* GS = GetGameState<AMultiplayerGameStateBase>())
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER HIT GAME MODE"));
		GS->PlayerHit();
	}
}

void AMultiplayerTestGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	M_Players.Add(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
		FString::Printf(TEXT("Player Joined: %s"), *M_Players.Last()->GetName()));
}

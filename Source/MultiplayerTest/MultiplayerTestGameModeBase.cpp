// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerTestGameModeBase.h"

#include "GameplayPlayerController.h"
#include "MultiplayerGameStateBase.h"
#include "Actors/GameplayActor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerTestGameModeBase::AMultiplayerTestGameModeBase()
{
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/BP_GameplayCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AMultiplayerTestGameModeBase::PlayerHit()
{
	if (AMultiplayerGameStateBase* GS = GetGameState<AMultiplayerGameStateBase>())
	{
		GS->PlayerHit();
	}
}

void AMultiplayerTestGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	M_Players.Add(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		FString::Printf(TEXT("Player Joined: %s"), *M_Players.Last()->GetName()));
}

void AMultiplayerTestGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (APlayerController* PC = Cast<APlayerController>(Exiting))
	{
		M_Players.Remove(PC);
	}
}

void AMultiplayerTestGameModeBase::RespawnPlayer(APlayerController* PlayerToRespawn)
{
	if (APawn* PlayerPawn = PlayerToRespawn->GetPawn())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
		FString::Printf(TEXT("Controller has a pawn, destroying it")));

		PlayerPawn->Destroy();
	}
	
	if (UWorld* World = GetWorld())
	{
		FindPlayerStarts();
		
		if (PlayerStarts.Num() > 0)
		{
			if (AGameplayPlayerController* GPC = Cast<AGameplayPlayerController>(PlayerToRespawn))
			{
				APlayerStart* SpawnPoint = GetPlayerSpawnPoint();
				AGameplayActor* SpawnedActor = World->SpawnActor<AGameplayActor>(BP_PlayerActor,
					SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
				GPC->M_PossessedPawn = SpawnedActor;
				PlayerToRespawn->Possess(SpawnedActor);
			}
		}
	}
}

//* PLAYER START STUFF
void AMultiplayerTestGameModeBase::FindPlayerStarts()
{
	if (const UWorld* World = GetWorld())
	{
		if (PlayerStarts.Num() == 0)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundActors);
			for (AActor* Actor : FoundActors)
			{
				if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
				{
					PlayerStarts.Add(PlayerStart);
				}
			}
		}
	}
}

APlayerStart* AMultiplayerTestGameModeBase::GetPlayerSpawnPoint()
{
	if (PlayerStarts.Num() > 0)
	{
		const int PlayerStartIndex = FMath::RandRange(0, PlayerStarts.Num() -1);
		return PlayerStarts[PlayerStartIndex];
	}
	else { return nullptr; }
}
//*
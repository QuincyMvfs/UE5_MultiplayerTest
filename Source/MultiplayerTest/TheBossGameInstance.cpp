// Fill out your copyright notice in the Description page of Project Settings.

#include "TheBossGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "GameplayPlayerController.h"

UTheBossGameInstance::UTheBossGameInstance()
{
	
}

void UTheBossGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTheBossGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTheBossGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTheBossGameInstance::OnJoinSessionComplete);
		}
	}
}

void UTheBossGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Levels/DefaultMap?listen");
		OnLobbyCreatedSuccessfullyEvent.Broadcast();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Session Created"));

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Session Failed to Create"));
		OnLobbyCreatedFailureEvent.Broadcast();
	}
}

void UTheBossGameInstance::OnFindSessionComplete(bool Succeeded)
{
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num())
		{
			OnAttemptJoinLobbyEvent.Broadcast();
			SessionInterface->JoinSession(0, FName("The BOSS Server"), SearchResults[0]);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Find Session Success"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Join Lobby Fail"));
		OnJoinLobbyFailureEvent.Broadcast();
	}
}

void UTheBossGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Join Lobby Success"));
			OnJoinLobbySuccessEvent.Broadcast();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Join Lobby Fail | Join Address = NULL"));
			OnJoinLobbyFailureEvent.Broadcast();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Join Lobby Fail | Player Controller = NULL"));
		OnJoinLobbyFailureEvent.Broadcast();
	}
}

void UTheBossGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Server"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 20000;
	SessionSettings.bAllowInvites = true;
	SessionSettings.BuildUniqueId = 5666;
	SessionSettings.bUseLobbiesIfAvailable = true;

	SessionInterface->CreateSession(0, FName("The BOSS Server"), SessionSettings);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attempt Session Create"));
	OnLobbyCreatedStartEvent.Broadcast();
}

void UTheBossGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 20000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attempt Join Session"));
	OnJoinLobbyStartedEvent.Broadcast();
}

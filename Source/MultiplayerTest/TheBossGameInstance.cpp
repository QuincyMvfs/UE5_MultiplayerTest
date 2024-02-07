// Fill out your copyright notice in the Description page of Project Settings.

#include "TheBossGameInstance.h"
#include "EngineUtils.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
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
		int32 ArrayIndex = -1;
		for (FOnlineSessionSearchResult SearchResult: SessionSearch->SearchResults)
		{
			ArrayIndex++;
			if (!SearchResult.IsValid()) continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Server Name";
			SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			SearchResult.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);
			
			Info.ServerName = ServerName;
			Info.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Info.ServerArrayIndex = ArrayIndex;
			Info.IsLan = SearchResult.Session.SessionSettings.bIsLANMatch;
			Info.Ping = SearchResult.PingInMs;
			Info.SetPlayerCount();
			
			OnFoundLobbyEvent.Broadcast(Info);
			
			//SessionInterface->JoinSession(0, M_SessionName, SearchResult);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Find Session Success, Attempting Join...."));

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Join Lobby Fail"));
		OnJoinLobbyFailureEvent.Broadcast();
	}

	OnServerSearchingEvent.Broadcast(false);
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

void UTheBossGameInstance::CreateServer(FCreateServerInfo InputServerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Server"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	// InputServerInfo.IsLan would be put here.
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.bAllowInvites = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings.bAntiCheatProtected = false;
	SessionSettings.bUsesStats = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = InputServerInfo.MaxPlayers;
	SessionSettings.NumPrivateConnections  = InputServerInfo.MaxPlayers;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), InputServerInfo.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	// SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, M_SessionName, SessionSettings);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attempt Session Create"));
	OnLobbyCreatedStartEvent.Broadcast();
}

void UTheBossGameInstance::FindServers()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	OnServerSearchingEvent.Broadcast(true);
}

void UTheBossGameInstance::JoinServer(int32 ArrayIndex)
{
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
	if (Result.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("JOINING SERVER"));
		SessionInterface->JoinSession(0, M_SessionName, Result);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to Join"));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "TheBossGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		FString ServerName;
	UPROPERTY(BlueprintReadOnly)
		FString PlayerCountStr;
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 MaxPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 ServerArrayIndex;

	void SetPlayerCount()
	{
		PlayerCountStr =
			(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}
};

//* Game Instance Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoundLobby, FServerInfo, ServerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerSearching, bool, IsSearchingForServer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyCreatedStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyCreatedSuccessfully);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyCreatedFailure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoinLobbyStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoinLobbySuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoinLobbyFailure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionFailure);
//*

UCLASS()
class MULTIPLAYERTEST_API UTheBossGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTheBossGameInstance();

public:
	UPROPERTY(BlueprintAssignable)
	FOnServerSearching OnServerSearchingEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnFoundLobby OnFoundLobbyEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnLobbyCreatedStart OnLobbyCreatedStartEvent;

	UPROPERTY(BlueprintAssignable)
	FOnLobbyCreatedSuccessfully OnLobbyCreatedSuccessfullyEvent;

	UPROPERTY(BlueprintAssignable)
	FOnLobbyCreatedFailure OnLobbyCreatedFailureEvent;

	UPROPERTY(BlueprintAssignable)
	FOnJoinLobbyStarted OnJoinLobbyStartedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnJoinLobbySuccess OnJoinLobbySuccessEvent;

	UPROPERTY(BlueprintAssignable)
	FOnJoinLobbyFailure OnJoinLobbyFailureEvent;

	UPROPERTY(BlueprintAssignable)
	FOnCreateSessionSuccess OnCreateSessionSuccessEvent;

	UPROPERTY(BlueprintAssignable)
	FOnCreateSessionFailure OnCreateSessionFailureEvent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Settings")
	float MouseSensitivity = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Settings")
	float ScopedMouseSensitivity = 0.35f;

protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName, FString HostName);

	UFUNCTION(BlueprintCallable)
	void FindServers();

	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 ArrayIndex);

	FName M_SessionName = "TheBOSSServer";
};

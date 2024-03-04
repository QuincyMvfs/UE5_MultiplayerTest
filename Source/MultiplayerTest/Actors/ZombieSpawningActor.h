// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerTest/Structs/FWave.h"
#include "ZombieSpawningActor.generated.h"

struct FWave;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnZombie, AActor*, SpawnedZombie, FVector, SpawnLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZombieKilled, AActor*, KilledZombie, FVector, DeathLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStarted, int, CurrentWave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveEnded, int, PreviousWave, int, NextWave);

UCLASS()
class MULTIPLAYERTEST_API AZombieSpawningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawningActor();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_StartWave();
	void Server_StartWave_Implementation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWave> M_Waves;

private:
	UPROPERTY(Replicated)
	int M_RemainingEnemiesCount = 0;

	UPROPERTY(Replicated)
	TArray<AActor*> M_SpawnedEnemies;
};

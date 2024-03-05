// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawningActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AZombieSpawningActor::AZombieSpawningActor()
{

}

// Called when the game starts or when spawned
void AZombieSpawningActor::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieSpawningActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombieSpawningActor, M_RemainingEnemiesCount);
	DOREPLIFETIME(AZombieSpawningActor, M_SpawnedEnemies);
}

void AZombieSpawningActor::StartWave()
{
	if (HasAuthority()) Server_StartWave();
}

void AZombieSpawningActor::Server_StartWave_Implementation()
{
	for (FWave Wave : M_Waves)
	{
		M_RemainingEnemiesCount += Wave.WaveEnemies.Num();
	}
}



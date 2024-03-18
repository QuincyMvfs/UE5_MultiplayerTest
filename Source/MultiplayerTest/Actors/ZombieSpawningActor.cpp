// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawningActor.h"

#include "BaseZombieAICharacter.h"
#include "MultiplayerTest/Components/HealthComponent.h"
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
	if (M_RemainingEnemiesCount > 0 || M_CurrentWave == M_Waves.Num()) return;
		
	UWorld* World = GetWorld();
	for (int i = 0; i < M_Waves[M_CurrentWave].WaveEnemies.Num(); i++)
	{
		if (World)
		{
			M_RemainingEnemiesCount++;
			AActor* SpawnPoint = GetSpawnPoint();
			FActorSpawnParameters SpawnParameters;
			AActor* SpawnedActor = World->SpawnActor<AActor>(M_Waves[M_CurrentWave].WaveEnemies[i],
				SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), SpawnParameters);
			if (ABaseZombieAICharacter* SpawnedZombie = Cast<ABaseZombieAICharacter>(SpawnedActor))
			{
				SpawnedZombie->M_HealthComponent->OnKilledEvent.AddDynamic(this, &AZombieSpawningActor::Server_RemoveSpawnedActor);
				UE_LOG(LogTemp, Warning, TEXT("Casts"));
			}

			M_SpawnedEnemies.Add(SpawnedActor);
		}
	}

	M_CurrentWave++;
}

AActor* AZombieSpawningActor::GetSpawnPoint()
{
	const int RandInt = FMath::RandRange(0, M_SpawnPoints.Num() - 1);
	return M_SpawnPoints[RandInt];
}

void AZombieSpawningActor::RemoveSpawnedActor(AActor* Spawned, AActor* Killer)
{
	if (M_SpawnedEnemies.Contains(Spawned))
	{
		M_SpawnedEnemies.Remove(Spawned);
		M_RemainingEnemiesCount--;
		UE_LOG(LogTemp, Warning, TEXT("Killed: %s | Remaining Enemies: %d"), *Spawned->GetName(), M_RemainingEnemiesCount)
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dosent Contain")); 
	}
}

void AZombieSpawningActor::Server_RemoveSpawnedActor_Implementation(AActor* Spawned, AActor* Killer)
{
	if (M_SpawnedEnemies.Contains(Spawned))
	{
		M_SpawnedEnemies.Remove(Spawned);
		M_RemainingEnemiesCount--;
		UE_LOG(LogTemp, Warning, TEXT("Killed: %s | Remaining Enemies: %d"), *Spawned->GetName(), M_RemainingEnemiesCount)
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dosent Contain")); 
	}
}



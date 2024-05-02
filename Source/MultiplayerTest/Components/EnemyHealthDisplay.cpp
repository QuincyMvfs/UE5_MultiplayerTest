// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthDisplay.h"

#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UEnemyHealthDisplay::UEnemyHealthDisplay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyHealthDisplay::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle CheckForEnemiesTimer;
	GetWorld()->GetTimerManager().SetTimer(
			CheckForEnemiesTimer, this, &UEnemyHealthDisplay::CheckForEnemies, 0.5f, true);
}

void UEnemyHealthDisplay::CheckForEnemies()
{
	if (M_CameraComponent)
	{
		const FVector startPoint = M_CameraComponent->GetComponentLocation();
		const FVector endPoint = startPoint + (M_CameraComponent->GetForwardVector() * m_rayLength);
		
		FHitResult hitResult;
		if (bool didHitObject = UKismetSystemLibrary::LineTraceSingle(
		this, startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		true, TArray<AActor*>(), EDrawDebugTrace::None, hitResult, true))
		{
			if (AActor* HitActor = hitResult.GetActor())
			{
				if (UHealthComponent* Health = HitActor->FindComponentByClass<UHealthComponent>())
				{
					DisplayHealthBar(HitActor, Health);
				}
				else { RemoveHealthBar(); }
			}
			else { RemoveHealthBar(); }
		}
		else { RemoveHealthBar(); }
	}
}

void UEnemyHealthDisplay::DisplayHealthBar(AActor* SpottedEntity, UHealthComponent* HealthComponent)
{
	if (HealthComponent->M_IsDead) return;
	
	M_CanSeeEnemy = true;
	M_SpottedEnemy = SpottedEntity;

	const FString EnumName = UEnum::GetValueAsString(HealthComponent->M_EntityType);
	const FText EnumText = FText::FromString(EnumName);
	OnEnemySpottedEvent.Broadcast(EnumText, HealthComponent);
}

void UEnemyHealthDisplay::RemoveHealthBar()
{
	M_SpottedEnemy = nullptr;
	M_CanSeeEnemy = false;
	OnNoEnemySpottedEvent.Broadcast();
}


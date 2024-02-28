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

void UEnemyHealthDisplay::BeginPlay() { Super::BeginPlay(); }


// Called every frame
void UEnemyHealthDisplay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForEnemies();
}

void UEnemyHealthDisplay::CheckForEnemies()
{
	if (m_cameraComponent)
	{
		const FVector startPoint = m_cameraComponent->GetComponentLocation();
		const FVector endPoint = startPoint + (m_cameraComponent->GetForwardVector() * m_rayLength);
		
		FHitResult hitResult;
		if (bool didHitObject = UKismetSystemLibrary::LineTraceSingle(
		this, startPoint, endPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		true, TArray<AActor*>(), EDrawDebugTrace::None, hitResult, true))
		{
			if (AActor* HitActor = hitResult.GetActor())
			{
				if (UHealthComponent* Health = HitActor->FindComponentByClass<UHealthComponent>())
				{
					M_CanSeeEnemy = true;
				}
				else { M_CanSeeEnemy = false; }
			}
			else { M_CanSeeEnemy = false; }
		}
		else { M_CanSeeEnemy = false; }
	}
}

void UEnemyHealthDisplay::DisplayHealthBar()
{
}

void UEnemyHealthDisplay::RemoveHealthBar()
{
}


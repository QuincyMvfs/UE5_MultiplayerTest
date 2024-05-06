// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyHealthDisplay.generated.h"


class UHealthComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemySpotted, FText, EnemyName, UHealthComponent*, HealthComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoEnemySpotted);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UEnemyHealthDisplay : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyHealthDisplay();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void CheckForEnemies();
	virtual void DisplayHealthBar(AActor* SpottedEntity, UHealthComponent* HealthComponent);
	virtual void RemoveHealthBar();

	UPROPERTY(BlueprintAssignable)
	FEnemySpotted OnEnemySpottedEvent;

	UPROPERTY(BlueprintAssignable)
	FNoEnemySpotted OnNoEnemySpottedEvent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool M_CanSeeEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* M_SpottedEnemy;

	UPROPERTY(EditAnywhere)
	UCameraComponent* M_CameraComponent;
	
private:
	const float m_rayLength = 10000;
	const FString& m_enumName = "EEnemyTypes::";
};

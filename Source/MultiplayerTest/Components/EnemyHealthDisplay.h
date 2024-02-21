// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyHealthDisplay.generated.h"


class UCameraComponent;

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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void CheckForEnemies();
	virtual void DisplayHealthBar();
	virtual void RemoveHealthBar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool M_CanSeeEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* M_SpottedEnemy;
	
private:
	UCameraComponent* m_cameraComponent;
	const float m_rayLength = 10000;
};

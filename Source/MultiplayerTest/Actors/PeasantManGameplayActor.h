// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayActor.h"
#include "PeasantManGameplayActor.generated.h"

class UShotgunComponent;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API APeasantManGameplayActor : public AGameplayActor
{
	GENERATED_BODY()

public:
	APeasantManGameplayActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* M_ShotgunStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UShotgunComponent* M_ShotgunComponent;

public:
	virtual void BeginPlay() override;

	virtual void Multi_SetShooting_Implementation(bool Value) override;

	virtual void Reload() override;

	virtual void GetAnimationVariables(bool& bIsFalling, bool& bIsAiming, bool& bIsShooting,
		bool& bIsReloading, bool& bIsHit, float& CurrentSpeed, FVector& CurrentVelocity,
		EMovementStates& CurrentState) override;

private:
	void RecoilTimer();
	void ShootingTimer();

	bool m_canShoot = true;
};

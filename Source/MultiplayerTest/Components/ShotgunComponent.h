// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponComponent.h"
#include "ShotgunComponent.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UShotgunComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()

public:
	virtual void ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation) override;

	virtual void Server_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation) override;

	virtual void Multi_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Variables")
	int M_TotalPellets = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Variables")
	float M_RecoilSpread = 10;

private:
	FVector CalculateInaccuracy(const UCameraComponent* CameraComponent);
};

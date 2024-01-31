// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseWeaponComponent.generated.h"


class AGrenadeProjectile;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UBaseWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool TryShootWeapon();
	virtual void ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter);
	virtual void TryReload();
	virtual void Reload();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	float M_Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	float M_DelayBetweenShots = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	float M_ReloadDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	int M_MaxMagazineCapacity = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Weapon Variables")
	bool M_IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	TSubclassOf<class AActor> M_GrenadeActor;
	
protected:
	virtual void PerformRaycast(FVector startPoint, FVector endPoint, AActor* shooter);

	
protected:
	int m_currentMagazine;
	float m_nextTimeToShoot;
	float m_rayLength = 1000.0f;
	bool m_canFire;
	
};

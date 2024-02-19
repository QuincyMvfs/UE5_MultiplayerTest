// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "BaseWeaponComponent.generated.h"


class UHealthComponent;
class USoundCue;
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
	virtual void BeginPlay() override;

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// SHOOTING CLIENT + SERVER
	virtual bool TryShootWeapon();
	virtual void ShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation);
	
	UFUNCTION(Server, Reliable)
	void Server_OnShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation);
	void Server_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_OnShootWeapon(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation);
	void Multi_OnShootWeapon_Implementation(UCameraComponent* cameraComponent, AActor* shooter, FVector muzzleLocation);
	// END OF SHOOTING FUNCTIONS
	
	// RELOADING CLIENT + SERVER
	virtual void TryReload();
	virtual void Reload();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryReload();
	bool Server_TryReload_Validate();
	void Server_TryReload_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_TryReload();
	bool Multi_TryReload_Validate();
	void Multi_TryReload_Implementation();

	void DealDamage(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DealDamage(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);
	bool Server_DealDamage_Validate(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);
	void Server_DealDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_DealDamage(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);
	bool Multi_DealDamage_Validate(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);
	void Multi_DealDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim, UHealthComponent* HitHealth, FName HitBone);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	USoundBase* M_FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	float M_Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	float M_DelayBetweenShots = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	float M_ReloadDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	int M_MaxMagazineCapacity = 30;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Base Weapon Variables")
	bool M_IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	TSubclassOf<class AActor> M_GrenadeActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon Variables")
	UNiagaraSystem* M_BulletTracer;

protected:
	virtual FVector PerformRaycast(FVector startPoint, FVector endPoint, AActor* shooter);
	virtual void SpawnBulletTracer(FVector startPoint, FVector endPoint, FRotator rotation);
	
protected:
	UPROPERTY(Replicated)
		FVector m_startPoint;
	UPROPERTY(Replicated)
		FVector m_forwardVector;
	UPROPERTY(Replicated)
		FVector m_rayEndPoint;
	UPROPERTY(Replicated)
		FVector m_hitEndPoint;
	UPROPERTY(Replicated)
		FVector m_muzzleLocation;

	APawn* m_pawnOwner;
	int m_currentMagazine;
	float m_nextTimeToShoot;
	float m_rayLength = 100000.0f;
	bool m_canFire;
};

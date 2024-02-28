// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AMultiplayerGameStateBase;
enum class ETeams : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamaged, float, HealthPercent, AActor*, Damaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKilled, AActor*, Killed, AActor*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunComplete);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDamaged OnDamagedEvent;
	UPROPERTY(BlueprintAssignable)
		FOnStunComplete OnStunCompletedEvent;
	UPROPERTY(BlueprintAssignable)
		FOnKilled OnKilledEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> M_BoneMultiplier;
	
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void TakeDamage(float Amount, AActor* Instigator, AActor* Victim, FName HitBone);

	// UFUNCTION(Server, Reliable)
	// void Server_TakeDamage(float Amount, AActor* Instigator, AActor* Victim, FName HitBone);
	// void Server_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim, FName HitBone);
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_TakeDamage(float Amount, AActor* Instigator, AActor* Victim, FName HitBone);
	bool Multi_TakeDamage_Validate(float Amount, AActor* Instigator, AActor* Victim, FName HitBone);
	void Multi_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim, FName HitBone);

	float GetMultipliedDamage(float BaseDamage, FName HitBone);
	
	void SetIsHit();
	void SetIsNotHit();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetIsNotHit();
	void Multi_SetIsNotHit_Implementation();

	UFUNCTION()
	void SendDamageDealtValues(AActor* Instigator, float Amount);

	UFUNCTION(Server, Reliable)
	void Server_SendDamageDealtValues(AActor* Instigator, float Amount);
	void Server_SendDamageDealtValues_Implementation(AActor* Instigator, float Amount);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float M_MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float M_HitDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	ETeams M_Team;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool M_IsHit = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool M_IsDead = false;
	
protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float m_currentHealth;

	AMultiplayerGameStateBase* m_gameStateRef;

	APawn* m_pawnOwner;
};

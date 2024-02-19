// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseZombieAICharacter.generated.h"

class UHealthComponent;
class USphereComponent;
class UPawnSensingComponent;

UCLASS()
class MULTIPLAYERTEST_API ABaseZombieAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseZombieAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* M_CapsuleComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* M_SkeletalMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UPawnSensingComponent* M_PawnSensingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCharacterMovementComponent* M_CharacterMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USphereComponent* M_AttackSphereCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UHealthComponent* M_HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual void GetAIAnimationVariables(bool& bIsHit, bool& bIsDead);
	
	UFUNCTION(BlueprintCallable)
	virtual void AIAttack();

	UFUNCTION(Server, Reliable)
	virtual void Server_AIAttack();
	virtual void Server_AIAttack_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_AIAttack();
	virtual void Multi_AIAttack_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual void Dead(AActor* Killer);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float M_Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float M_AttackSpeed = 1.5f;
	
};

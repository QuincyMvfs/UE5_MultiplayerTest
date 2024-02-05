// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

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
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void TakeDamage(float Amount, AActor* Instigator, AActor* Victim);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_TakeDamage(float Amount, AActor* Instigator, AActor* Victim);
	bool Multi_TakeDamage_Validate(float Amount, AActor* Instigator, AActor* Victim);
	void Multi_TakeDamage_Implementation(float Amount, AActor* Instigator, AActor* Victim);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float M_MaxHealth = 100.0f;

protected:
	UPROPERTY(Replicated)
	float m_currentHealth;
};

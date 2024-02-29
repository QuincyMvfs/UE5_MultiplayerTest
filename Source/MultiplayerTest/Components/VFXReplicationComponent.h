// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VFXReplicationComponent.generated.h"

class UParticleEmitter;
class UNiagaraComponent;

UENUM(BlueprintType)
enum class ENiagaraType : uint8
{
	AtLocation UMETA(DisplayName = "AtLocation"),
	SystemAttached UMETA(DisplayName = "SystemAttached"),
	AtLocationEndPoint UMETA(DisplayName = "AtLocationEndPoint")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UVFXReplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVFXReplicationComponent();

	UParticleSystemComponent* M_LegacyParticleSystemComponent;
	UNiagaraComponent* M_NiagaraSystemComponent;

public:
	//* NIAGARA EFFECT
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnNiagara(ENiagaraType NiagaraType, UNiagaraSystem* NiagaraSystem,
		USceneComponent* AttachToComponent, FName AttachPointName = NAME_None,
		FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator,
		bool bAutoDestroy = true);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_SpawnNiagara(ENiagaraType NiagaraType, UNiagaraSystem* NiagaraSystem,
		USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, 
		FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator,
		bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CancelNiagaraEffect();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_CancelNiagaraEffect();
	//* 

	//* LEGACY EMITTER 
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnEmitter(ENiagaraType EmitterType, UParticleSystem* ParticleSystem,
		USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector::ZeroVector,
		FRotator Rotation = FRotator::ZeroRotator, bool bAutoDestroy = true);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SpawnEmitter(ENiagaraType EmitterType, UParticleSystem* UParticleSystem,
		USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector::ZeroVector,
		FRotator Rotation = FRotator::ZeroRotator, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CancelEmitterEffect();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_CancelEmitterEffect();
	//*
	
};

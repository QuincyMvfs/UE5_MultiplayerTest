// Fill out your copyright notice in the Description page of Project Settings.


#include "VFXReplicationComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UVFXReplicationComponent::UVFXReplicationComponent()
{
	
}

void UVFXReplicationComponent::Server_SpawnNiagara_Implementation(ENiagaraType NiagaraType,
	UNiagaraSystem* NiagaraSystem, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location,
	FRotator Rotation, bool bAutoDestroy)
{
	Multi_SpawnNiagara(NiagaraType, NiagaraSystem, AttachToComponent,
		AttachPointName, Location, Rotation, bAutoDestroy);
}

void UVFXReplicationComponent::Multi_SpawnNiagara_Implementation(ENiagaraType NiagaraType,
	UNiagaraSystem* NiagaraSystem, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location,
	FRotator Rotation, bool bAutoDestroy)
{
	if (!NiagaraSystem) return;

	if (UWorld* World = GetWorld())
	{
		switch (NiagaraType)
		{
			case ENiagaraType::AtLocation:
				M_NiagaraSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, NiagaraSystem, Location,
					Rotation, FVector::One(), bAutoDestroy);
				break;
			
			case ENiagaraType::SystemAttached:
				M_NiagaraSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem,
					AttachToComponent, AttachPointName, Location, Rotation,
					EAttachLocation::KeepWorldPosition, bAutoDestroy, true);
				break;
			
			default:
				break;
		}
	}
}

void UVFXReplicationComponent::Server_CancelNiagaraEffect_Implementation()
{
	Multi_CancelNiagaraEffect();
}

void UVFXReplicationComponent::Multi_CancelNiagaraEffect_Implementation()
{
	M_NiagaraSystemComponent->DeactivateImmediate();
}

// LEGACY EMITTER SPAWNER
void UVFXReplicationComponent::Server_SpawnEmitter_Implementation(ENiagaraType EmitterType,
	UParticleSystem* ParticleSystem, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location,
	FRotator Rotation, bool bAutoDestroy)
{
	Multi_SpawnEmitter(EmitterType, ParticleSystem, AttachToComponent,
		AttachPointName, Location, Rotation, bAutoDestroy);
}

void UVFXReplicationComponent::Multi_SpawnEmitter_Implementation(ENiagaraType EmitterType,
	UParticleSystem* ParticleSystem, USceneComponent* AttachToComponent, FName AttachPointName,
	FVector Location,FRotator Rotation, bool bAutoDestroy)
{
	if (!ParticleSystem) return;
	
	if (UWorld* World = GetWorld())
	{
		switch (EmitterType)
		{
			case ENiagaraType::AtLocation:
				M_LegacyParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(World, ParticleSystem, Location,
					Rotation, FVector::OneVector, bAutoDestroy, EPSCPoolMethod::None, true);
				break;
					
			case ENiagaraType::SystemAttached:
				M_LegacyParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(ParticleSystem, AttachToComponent,
					AttachPointName, Location, Rotation, FVector::OneVector, EAttachLocation::KeepWorldPosition,
					bAutoDestroy, EPSCPoolMethod::None, true);
				break;

			default:
				break;
		}
	}
}

void UVFXReplicationComponent::Server_CancelEmitterEffect_Implementation()
{
	Multi_CancelEmitterEffect();
}

void UVFXReplicationComponent::Multi_CancelEmitterEffect_Implementation()
{
	M_LegacyParticleSystemComponent->DeactivateImmediate();
}

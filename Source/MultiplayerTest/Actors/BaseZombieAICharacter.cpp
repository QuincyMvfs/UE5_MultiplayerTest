// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseZombieAICharacter.h"

#include "Components/SphereComponent.h"
#include "MultiplayerTest/Components/HealthComponent.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ABaseZombieAICharacter::ABaseZombieAICharacter()
{
 	M_CapsuleComponent = GetCapsuleComponent();
	M_SkeletalMeshComponent = GetMesh();
	M_PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	M_CharacterMovementComponent = GetCharacterMovement();
	M_AttackSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphereCollider"));
	M_AttackSphereCollider->SetupAttachment(M_SkeletalMeshComponent, TEXT("RightHand"));
	M_HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABaseZombieAICharacter::BeginPlay() { Super::BeginPlay(); }

void ABaseZombieAICharacter::AIAttack()
{
	TArray<AActor*> OverlappingActors;
	M_AttackSphereCollider->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (UHealthComponent* HealthComponent = Actor->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->TakeDamage(M_Damage, this, Actor, "NULL");
		}
	}
}



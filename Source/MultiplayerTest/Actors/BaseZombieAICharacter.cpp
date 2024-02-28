// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseZombieAICharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "MultiplayerTest/Components/HealthComponent.h"
#include "MultiplayerTest/EnumClasses/ETeams.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ABaseZombieAICharacter::ABaseZombieAICharacter()
{
	// Defaults
 	M_CapsuleComponent = GetCapsuleComponent();
	M_SkeletalMeshComponent = GetMesh();
	M_PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	M_CharacterMovementComponent = GetCharacterMovement();
	
	// Melee Collider
	M_AttackSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphereCollider"));
	M_AttackSphereCollider->SetupAttachment(M_SkeletalMeshComponent, TEXT("RightHand"));
	
	// Health
	M_HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	M_HealthComponent->M_Team = ETeams::Zombies;
	M_HealthComponent->OnKilledEvent.AddDynamic(this, &ABaseZombieAICharacter::Dead);
}

// Called when the game starts or when spawned
void ABaseZombieAICharacter::BeginPlay() { Super::BeginPlay(); }

void ABaseZombieAICharacter::GetAIAnimationVariables(bool& bIsHit, bool& bIsDead)
{
	bIsHit = M_HealthComponent->M_IsHit;
	bIsDead = M_HealthComponent->M_IsDead;
}

void ABaseZombieAICharacter::AIAttack()
{
	if (!HasAuthority() && IsLocallyControlled()) { Server_AIAttack(); }
	else { Multi_AIAttack(); }
}

void ABaseZombieAICharacter::Server_AIAttack_Implementation()
{
	Multi_AIAttack();
}

void ABaseZombieAICharacter::Multi_AIAttack_Implementation()
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

void ABaseZombieAICharacter::Dead(AActor* Killed, AActor* Killer)
{
	M_CapsuleComponent->SetCollisionProfileName("DeadPlayer");
}



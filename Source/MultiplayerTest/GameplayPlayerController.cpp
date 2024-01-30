// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayPlayerController.h"

#include "Actors/GameplayActor.h"

AGameplayPlayerController::AGameplayPlayerController()
{
}

void AGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	M_PossesedPawn = Cast<AGameplayActor>(GetPawn());
}

void AGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AGameplayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGameplayPlayerController::MoveForward(float Value)
{
	
}

void AGameplayPlayerController::MoveRight(float Value)
{
	
}

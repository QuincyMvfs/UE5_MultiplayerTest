// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayPlayerController.generated.h"

class AGameplayActor;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API AGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGameplayPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);


	AGameplayActor* M_PossesedPawn;
};

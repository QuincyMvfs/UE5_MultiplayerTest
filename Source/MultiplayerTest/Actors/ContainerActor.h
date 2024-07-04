// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ContainerActor.generated.h"

class UContainerComponent;

UCLASS()
class MULTIPLAYERTEST_API AContainerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AContainerActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UContainerComponent* M_ContainerComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

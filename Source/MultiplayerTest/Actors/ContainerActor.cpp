// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerActor.h"

#include "MultiplayerTest/Components/ContainerComponent.h"

// Sets default values
AContainerActor::AContainerActor()
{
	M_ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));

}

// Called when the game starts or when spawned
void AContainerActor::BeginPlay()
{
	Super::BeginPlay();
	
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EMovementStates.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMovementStates:uint8
{
	Idle,
	Walking,
	Running,
	Sprinting,
	Crouching,
	Jumping,
	Dead
};


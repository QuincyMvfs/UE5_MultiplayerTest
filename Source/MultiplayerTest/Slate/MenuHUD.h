// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class SMainMenuWidget;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

	TSharedPtr<SMainMenuWidget> MenuWidget;
	TSharedPtr<SWidget> MenuWidgetContainer;

	virtual void BeginPlay() override;
	
};

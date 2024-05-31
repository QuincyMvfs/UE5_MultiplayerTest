// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"

#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GEngine && GEngine->GameViewport)
	{
		MenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer,
			SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
	}
}

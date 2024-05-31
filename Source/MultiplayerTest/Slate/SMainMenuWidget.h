// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MULTIPLAYERTEST_API SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget) {};
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD);
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	FReply OnPlayClicked() const;
	FReply OnQuitClicked() const;
	
	TWeakObjectPtr<AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainMenuWidget.h"

#include "MenuHUD.h"

#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	const FMargin ContentPadding = FMargin(500.0f, 300.0f);
	const FMargin ButtonPadding = FMargin(10.0f);

	const FText TitleText = LOCTEXT("GameTitle", "My Super Great Game");
	const FText PlayText = LOCTEXT("PlayGame", "PLAY");
	const FText SettingsText = LOCTEXT("Settings", "Settings");
	const FText QuitText = LOCTEXT("QuitGame", "quit");

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;

	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 60.0f;
	
	ChildSlot
	[
		// Black Background Overlay
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FColor::Black)
		]
		
		// Vertical Box
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding)
		[
			// Create Vertical Box
			SNew(SVerticalBox)
			
			// Title Text
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(TitleText)
				.Font(TitleTextStyle)
				.Justification(ETextJustify::Center)
			]

			// Play Button
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnPlayClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(PlayText)
						.Justification(ETextJustify::Center)
					]
				]

			// Settings Button
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(SettingsText)
						.Justification(ETextJustify::Center)
					]
				]

			// Quit Button
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnQuitClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(QuitText)
						.Justification(ETextJustify::Center)
					]
				]
		]
	];
}

FReply SMainMenuWidget::OnPlayClicked() const
{
	return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->ConsoleCommand("Quit");
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("QUIT"));

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

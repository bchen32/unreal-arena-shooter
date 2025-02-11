// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaShooterGameInstance.h"

void USettingsUserWidget::ChangeSensitivity()
{
	UArenaShooterGameInstance* ASGameInstance = Cast<UArenaShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (SensitivityInput)
	{
		// Update the editable text
		float SensitivitySliderValue = SensitivitySlider->GetValue();
		SensitivityInput->SetText(FText::AsNumber(SensitivitySliderValue));

		ASGameInstance->SetSensitivity(SensitivitySliderValue);
	}
}

void USettingsUserWidget::ChangeSensitivity_EditableInputParams(const FText& Text, ETextCommit::Type CommitMethod)
{
	UArenaShooterGameInstance* ASGameInstance = Cast<UArenaShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Change the sensitivity slider and the value in the game instance
	if (Text.ToString().IsNumeric())
	{
		if (SensitivitySlider)
		{
			float SensitivityValue = FCString::Atof(*(Text.ToString()));
			SensitivityValue = FMath::Clamp(SensitivityValue, SensitivitySlider->GetMinValue(), SensitivitySlider->GetMaxValue());
			// Update on slider
			SensitivitySlider->SetValue(SensitivityValue);

			// Update on editable text
			SensitivityInput->SetText(FText::AsNumber(SensitivityValue));

			// Update on the game instance
			ASGameInstance->SetSensitivity(SensitivityValue);
		}
	}
	else
	{
		// Has non-numbers, let's just set it to 1.0f;
		SensitivityInput->SetText(FText::FromString(FString(TEXT("1.0"))));

		// Update on game instance
		ASGameInstance->SetSensitivity(1.0f);
	}
}

void USettingsUserWidget::BackButtonPressed()
{
	OnBackButtonPressed.Broadcast();
}

void USettingsUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UArenaShooterGameInstance* ASGameInstance = Cast<UArenaShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Sensitivity setup
	if (SensitivitySlider)
	{
		// Set to old sensitivity value 
		if (ASGameInstance)
		{
			SensitivitySlider->SetValue(ASGameInstance->GetSensitivity());
		}
		else
		{
			// Default value if couldn't find old value
			SensitivitySlider->SetValue(1.0f);
		}
		
		// Add binding when user finishes changing slider
		SensitivitySlider->OnMouseCaptureEnd.AddDynamic(this, &USettingsUserWidget::ChangeSensitivity);
		SensitivitySlider->OnControllerCaptureEnd.AddDynamic(this, &USettingsUserWidget::ChangeSensitivity);
	}
	if (SensitivityInput)
	{
		// Set to old sensitivity value
		if (ASGameInstance)
		{
			SensitivityInput->SetText(FText::FromString(FString::SanitizeFloat(ASGameInstance->GetSensitivity())));
		}
		else
		{
			SensitivityInput->SetText(FText::FromString(FString(TEXT("1.0"))));
		}

		// Add binding when user finishes setting text
		SensitivityInput->OnTextCommitted.AddDynamic(this, &USettingsUserWidget::ChangeSensitivity_EditableInputParams);
	}

	// Back button
	if (BackButton)
	{
		BackButton->OnReleased.AddDynamic(this, &USettingsUserWidget::BackButtonPressed);
	}
}

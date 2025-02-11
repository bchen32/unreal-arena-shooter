// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Types/SlateEnums.h"
#include "SettingsUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBackButtonPressed);

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API USettingsUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void ChangeSensitivity();
	UFUNCTION()
	void ChangeSensitivity_EditableInputParams(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void BackButtonPressed();
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	/* SENSITIVITY GROUP */
	UPROPERTY(meta = (BindWidget))
	USlider* SensitivitySlider;
	UPROPERTY(meta = (BindWidget))
	UEditableText* SensitivityInput;

	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintAssignable)
	FBackButtonPressed OnBackButtonPressed;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class ARENASHOOTER_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;


	UFUNCTION(BlueprintCallable)
	void SetAmmo(int32 NewCurAmmo, int32 NewMaxAmmo);

};

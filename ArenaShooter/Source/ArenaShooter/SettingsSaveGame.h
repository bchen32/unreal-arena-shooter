// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	float Sensitivity;

	USettingsSaveGame();

};

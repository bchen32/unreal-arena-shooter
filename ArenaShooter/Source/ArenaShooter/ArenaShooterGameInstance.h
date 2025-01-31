// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ArenaShooterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API UArenaShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	float HighScore;
protected:
public:
	UArenaShooterGameInstance();

	UFUNCTION(BlueprintCallable)
	float GetHighScore() const { return HighScore; }
	UFUNCTION(BlueprintCallable)
	void SetHighScore(float NewHighScore);

};

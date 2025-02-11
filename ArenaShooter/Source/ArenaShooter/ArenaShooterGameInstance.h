// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UpgradeSystem.h"
#include "SettingsSaveGame.h"
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

	float Sensitivity;

	void SaveSettings();
protected:
public:
	UArenaShooterGameInstance();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    TArray<int32> UpgradeList;

	UFUNCTION(BlueprintCallable)
	float GetHighScore() const { return HighScore; }
	UFUNCTION(BlueprintCallable)
	void SetHighScore(float NewHighScore);
	
	float GetSensitivity() const { return Sensitivity; }
	void SetSensitivity(float NewSensitivity);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
    void Upgrade(EUpgradeType UpgradeType);

};

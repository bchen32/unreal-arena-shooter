// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UArenaShooterGameInstance::SaveSettings()
{
	if (USettingsSaveGame* SaveGameObject = Cast<USettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingsSaveGame::StaticClass())))
	{
		// Settings we are saving
		SaveGameObject->Sensitivity = Sensitivity;

		// Save it to disk
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameObject, FString(TEXT("Settings")), 0);
	}
}

UArenaShooterGameInstance::UArenaShooterGameInstance()
{
	HighScore = 0;
	Sensitivity = 1.0f;

	int32 NumUpgrades = static_cast<int32>(EUpgradeType::Max);
    UpgradeList.Init(0, NumUpgrades);

    // Testing purposes
    // UpgradeList[static_cast<int32>(EUpgradeType::DoubleJump)] = 0;
    // UpgradeList[static_cast<int32>(EUpgradeType::DoubleDash)] = 0;
    // UpgradeList[static_cast<int32>(EUpgradeType::SlowMo)] = 1;
}

void UArenaShooterGameInstance::SetHighScore(float NewHighScore)
{
	if (HighScore < NewHighScore)
	{
		HighScore = NewHighScore;
	}
}

void UArenaShooterGameInstance::SetSensitivity(float NewSensitivity)
{
	if (Sensitivity != NewSensitivity)
	{
		Sensitivity = NewSensitivity;
		SaveSettings();
	}
}

void UArenaShooterGameInstance::Init()
{
	// Load the settings
	if (USettingsSaveGame* SettingsSave = Cast<USettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("Settings")), 0)))
	{
		Sensitivity = SettingsSave->Sensitivity;
	}
	else
	{
		Sensitivity = 1.0f;
	}
}

void UArenaShooterGameInstance::Upgrade(EUpgradeType UpgradeType)
{
    int32 Index = static_cast<int32>(UpgradeType);
    if (Index >= 0 && Index < UpgradeList.Num())
    {
        UpgradeList[Index]++;
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooterGameInstance.h"

UArenaShooterGameInstance::UArenaShooterGameInstance()
{
	HighScore = 0;

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

void UArenaShooterGameInstance::Upgrade(EUpgradeType UpgradeType)
{
    int32 Index = static_cast<int32>(UpgradeType);
    if (Index >= 0 && Index < UpgradeList.Num())
    {
        UpgradeList[Index]++;
    }
}

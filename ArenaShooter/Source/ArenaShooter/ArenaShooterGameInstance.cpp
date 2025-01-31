// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooterGameInstance.h"

UArenaShooterGameInstance::UArenaShooterGameInstance()
{
	HighScore = 0;
}

void UArenaShooterGameInstance::SetHighScore(float NewHighScore)
{
	if (HighScore < NewHighScore)
	{
		HighScore = NewHighScore;
	}
}

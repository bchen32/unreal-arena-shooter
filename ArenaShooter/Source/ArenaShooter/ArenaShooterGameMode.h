// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaShooterGameMode.generated.h"

UCLASS(minimalapi)
class AArenaShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	float EnemiesKilled;

protected:
	UPROPERTY(EditAnywhere)
	float EnemiesKilledGoal;

public:
	AArenaShooterGameMode();


	virtual void StartPlay() override;
	virtual void EndLevel();

	void AddToKillGoal(int AdditionalKills);

	void KilledEnemy();
};




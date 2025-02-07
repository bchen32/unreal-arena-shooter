// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaShooterGameMode.h"
#include "ArenaShooterCharacter.h"
#include "ArenaShooterGameInstance.h"
#include "ArenaShooterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/EnemyTimerSpawner.h"
#include "UObject/ConstructorHelpers.h"

AArenaShooterGameMode::AArenaShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	EnemiesKilledGoal = 0;
	EnemiesKilled = 0;
}



void AArenaShooterGameMode::StartPlay()
{
	Super::StartPlay();

	if (AArenaShooterGameState* ArenaShooterGameState = GetGameState<AArenaShooterGameState>())
	{
		ArenaShooterGameState->SetScore(0);
	}

	// Temporary remove and make more modular
	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyTimerSpawner::StaticClass(), Spawners);
	for (AActor* Actor : Spawners)
	{
		if (AEnemyTimerSpawner* Spawner = Cast<AEnemyTimerSpawner>(Actor)) {
			AddToKillGoal(Spawner->GetSpawnCap());
		}
	}
}

void AArenaShooterGameMode::EndLevel()
{
	if (UArenaShooterGameInstance* ArenaShooterGameInstance = GetGameInstance<UArenaShooterGameInstance>())
	{
		if (AArenaShooterGameState* ArenaShooterGameState = GetGameState<AArenaShooterGameState>())
		{
			ArenaShooterGameInstance->SetHighScore(ArenaShooterGameState->GetScore());
		}
	}
}

void AArenaShooterGameMode::AddToKillGoal(int AdditionalKills)
{
	EnemiesKilledGoal += AdditionalKills;
}

void AArenaShooterGameMode::KilledEnemy()
{
	if (++EnemiesKilled == EnemiesKilledGoal)
	{
		EndLevel();
		UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuMap"));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTimerSpawner.h"

AEnemyTimerSpawner::AEnemyTimerSpawner()
{
	InitialDelay = 5.0f;
	Delay = 4.0f;
	Deviation = 1.0f;
	Enabled = true;
	SpawnCap = 10;
}

void AEnemyTimerSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (Enabled == true)
	{
		NumSpawned = 0;
		TimerLoop();
	}
}

void AEnemyTimerSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Disable();
}

void AEnemyTimerSpawner::Enable()
{
	if (Enabled == false)
	{
		NumSpawned = 0;
		Enabled = true;

		TimerLoop();
	}
}

void AEnemyTimerSpawner::Disable()
{
	if (Enabled == true)
	{
		Enabled = false;

		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void AEnemyTimerSpawner::TimerLoop()
{
	float DelayWithDeviation;
	if (NumSpawned == 0) {
		DelayWithDeviation = InitialDelay;
	}
	else {
		DelayWithDeviation = Delay;
	}
	DelayWithDeviation += FMath::RandRange(-Deviation, Deviation);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this,
		&AEnemyTimerSpawner::TimerLoopSpawn, DelayWithDeviation, false); // InFirstDelay not useful because not looping timer
}

void AEnemyTimerSpawner::TimerLoopSpawn()
{
	if (NumSpawned < SpawnCap) {
		Super::Spawn();
		++NumSpawned;
		TimerLoop();
	}
	else {
		Disable();
	}
}

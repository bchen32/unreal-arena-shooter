// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "EnemyController.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawner::Spawn()
{
	if (UWorld* World = GetWorld()) {
		AEnemy* Enemy = World->SpawnActorDeferred<AEnemy>(EnemySpawnType, FTransform(GetActorRotation(), GetActorLocation() + SpawnOffset));

		if (Enemy)
		{
			// Once spawned, get the designated controller for it
			AEnemyController* EnemyController = World->SpawnActorDeferred<AEnemyController>(Enemy->AIControllerClass, FTransform());
			if (EnemyController)
			{
				// Success, possess the enemy.
				EnemyController->Possess(Enemy);

				// Finalize spawning i.e. let BeginPlay() finally run.
				EnemyController->FinishSpawning(FTransform());
				Enemy->FinishSpawning(FTransform(GetActorRotation(), GetActorLocation() + SpawnOffset));
			}
			else
			{
				// Cleanup after ourselves if failed.
				Enemy->Destroy();
			}
		}
	}
}



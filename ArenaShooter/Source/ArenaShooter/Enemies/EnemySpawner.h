// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ARENASHOOTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
private:

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> EnemySpawnType = AEnemy::StaticClass();

	UPROPERTY(EditAnywhere)
	FVector SpawnOffset = FVector();

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	virtual void Spawn();

};

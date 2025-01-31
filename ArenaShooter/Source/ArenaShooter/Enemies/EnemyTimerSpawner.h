// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "EnemyTimerSpawner.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API AEnemyTimerSpawner : public AEnemySpawner
{
	GENERATED_BODY()
	
private:
	FTimerHandle SpawnTimerHandle;
	int NumSpawned;

	UFUNCTION()
	void TimerLoop();
	UFUNCTION()
	void TimerLoopSpawn();

protected:
	UPROPERTY(EditAnywhere)
	float InitialDelay;

	UPROPERTY(EditAnywhere)
	float Delay;

	UPROPERTY(EditAnywhere)
	float Deviation;

	UPROPERTY(EditAnywhere)
	bool Enabled;

	UPROPERTY(EditAnywhere)
	int SpawnCap;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	AEnemyTimerSpawner();
	virtual void Enable();
	virtual void Disable();

	int GetSpawnCap() const { return SpawnCap; }
};

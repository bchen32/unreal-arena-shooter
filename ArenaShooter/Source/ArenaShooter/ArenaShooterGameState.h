// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ArenaShooterGameState.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API AArenaShooterGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	float Score;
protected:
public:
	AArenaShooterGameState();

	float GetScore() const { return Score; }
	void SetScore(float NewScore) { Score = NewScore; }
	void AddScore(float AdditionalScore) { Score += AdditionalScore; }


};

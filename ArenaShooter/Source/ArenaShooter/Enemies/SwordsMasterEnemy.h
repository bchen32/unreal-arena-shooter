// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "SwordsMasterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API ASwordsMasterEnemy : public AEnemy
{
	GENERATED_BODY()
private:
protected:
	UPROPERTY(EditAnywhere)
	float LungePower;

public:
	ASwordsMasterEnemy();

	virtual void Attack() override;
};

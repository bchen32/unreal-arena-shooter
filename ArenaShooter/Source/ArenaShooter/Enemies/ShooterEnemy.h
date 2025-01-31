// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Projectile.h"
#include "Enemy.h"
#include "ShooterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API AShooterEnemy : public AEnemy
{
	GENERATED_BODY()
	
private:

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileType;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere)
	float MuzzleLateralOffset;

	UPROPERTY(EditAnywhere)
	float MuzzleZOffset;
public:
	AShooterEnemy();
	virtual void BeginPlay() override;

	virtual void Attack() override;
};

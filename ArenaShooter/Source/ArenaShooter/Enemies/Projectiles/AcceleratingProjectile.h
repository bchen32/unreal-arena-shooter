// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Projectiles/Projectile.h"
#include "AcceleratingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API AAcceleratingProjectile : public AProjectile
{
	GENERATED_BODY()

private:
	FTimerHandle AcceleratingTimerHandle;

	UFUNCTION()
	void IncreaseSpeed();
protected:
	UPROPERTY(EditAnywhere)
	float SpeedIncreaseDelay;
	UPROPERTY(EditAnywhere)
	float SpeedMultiplier;

	virtual void BeginPlay() override;
public:
	AAcceleratingProjectile();

	/** The input velocity will represent the maximum speed that this projectile can take. */
	virtual void Shoot(FVector Velocity) override;
	
	/** The delay between when the projectile increases its speed.*/
	void SetSpeedIncreaseDelay(float Delay) { SpeedIncreaseDelay = Delay; }
	float GetSpeedIncreaseDelay() const { return SpeedIncreaseDelay; }

	/** The amount two increase the projectile. This version multiplies the value to get the new value */
	void SetSpeedMultiplier(float Multiplier) { SpeedMultiplier = Multiplier; }
	float GetSpeedMultipier() const { return SpeedMultiplier; }

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Accessories/WeakSpotComponent.h"
#include "MultiplierWeakSpotComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Weak Spots"), meta = (BlueprintSpawnableComponent))
class ARENASHOOTER_API UMultiplierWeakSpotComponent : public UWeakSpotComponent
{
	GENERATED_BODY()
	
private:
protected:
	UPROPERTY(EditAnywhere)
	float DamageMultiplier;
public:
	UMultiplierWeakSpotComponent();

	virtual float ApplyDamageModifier(float OriginalDamage) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WeakSpotComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Weak Spots"), meta = (BlueprintSpawnableComponent))
class ARENASHOOTER_API UWeakSpotComponent : public USphereComponent
{
	GENERATED_BODY()
	
private:
protected:
public:
	UWeakSpotComponent();

	virtual float ApplyDamageModifier(float OriginalDamage);
};

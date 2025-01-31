// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Accessories/MultiplierWeakSpotComponent.h"
#include "MultiplierWeakSpotComponent.h"


UMultiplierWeakSpotComponent::UMultiplierWeakSpotComponent()
{
	DamageMultiplier = 2.0f;
}

float UMultiplierWeakSpotComponent::ApplyDamageModifier(float OriginalDamage)
{
	return DamageMultiplier * OriginalDamage;
}
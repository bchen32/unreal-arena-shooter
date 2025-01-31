// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Accessories/WeakSpotComponent.h"

UWeakSpotComponent::UWeakSpotComponent()
{
	// Prevent the collision of this component from activate the parent's
	SetNotifyRigidBodyCollision(false);
}

float UWeakSpotComponent::ApplyDamageModifier(float OriginalDamage)
{
	return OriginalDamage;
}
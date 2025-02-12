// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "Railgun.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API ARailgun : public AWeapon
{
	GENERATED_BODY()

public:
	ARailgun();

	virtual void StartReload() override;

	virtual void Enable() override;

	virtual void Disable() override;

	virtual void OnKill() override;

};

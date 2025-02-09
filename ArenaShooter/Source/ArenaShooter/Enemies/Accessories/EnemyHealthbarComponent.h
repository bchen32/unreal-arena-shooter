// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Enemies/Enemy.h"
#include "EnemyHealthbarComponent.generated.h"

/**
 * A singular healthbar component, specifically for Enemy, that updates based on the
 * Enemy's health.
 */
UCLASS(ClassGroup = ("Healthbars"))
class ARENASHOOTER_API UEnemyHealthbarComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	/** Quick pointers to objects as to not have to find again */
	UPROPERTY()
	UProgressBar* ProgressBar;
	
	UPROPERTY()
	AEnemy* EnemyOwner;

	virtual void BeginPlay() override;

	virtual void BindToHealth();

	UFUNCTION()
	virtual void UpdateHealthbarProgress(float OldHealth, float NewHealth);
public:
	UEnemyHealthbarComponent();

	UProgressBar* GetHealthProgressBar();
};

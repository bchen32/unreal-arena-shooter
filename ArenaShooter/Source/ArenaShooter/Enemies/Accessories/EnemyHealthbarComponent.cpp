// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Accessories/EnemyHealthbarComponent.h"

void UEnemyHealthbarComponent::UpdateHealthbarProgress(float OldHealth, float NewHealth)
{
	if (EnemyOwner)
	{
		if (UProgressBar* Healthbar = GetHealthProgressBar())
		{
			if (EnemyOwner->GetMaxHealth() != 0)
			{
				Healthbar->SetPercent(NewHealth / EnemyOwner->GetMaxHealth());
			}
		}
	}
}

UEnemyHealthbarComponent::UEnemyHealthbarComponent()
{
	SetGenerateOverlapEvents(false);

	ProgressBar = nullptr;
	EnemyOwner = nullptr;
}

void UEnemyHealthbarComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind the healthbar so it actually updates when the enemy owner changes health
	BindToHealth();
}

void UEnemyHealthbarComponent::BindToHealth()
{
	// If we are attached to enemy
	if ((EnemyOwner = Cast<AEnemy>(GetOwner())))
	{
		// Bind enemy's health change = change the progress bar
		EnemyOwner->OnHealthChanged.AddDynamic(this, &UEnemyHealthbarComponent::UpdateHealthbarProgress);
	}
}

UProgressBar* UEnemyHealthbarComponent::GetHealthProgressBar()
{
	// Let's see if we already have a progress bar, if so, quickly return that
	if (ProgressBar) return ProgressBar;

	// No progress bar, lets find it.
	UUserWidget* HealthbarWidget = GetUserWidgetObject();
	if (HealthbarWidget)
	{
		// If it properly casts, we have progress bar
		// if not, then its still null
		ProgressBar = Cast<UProgressBar>(HealthbarWidget->GetWidgetFromName(FName("Healthbar")));
	}

	return ProgressBar;
}

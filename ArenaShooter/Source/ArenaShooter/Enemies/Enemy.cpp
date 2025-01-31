// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "../ArenaShooterGameMode.h"
#include "../ArenaShooterGameState.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MaxHealth = Health = 30.0f;

	BehaviorTree = nullptr;
}

void AEnemy::SetHealth(float NewHealth)
{
	if (Health != NewHealth)
	{
		OnHealthChanged.Broadcast(Health, NewHealth);
		Health = NewHealth;
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetHealth(GetHealth() - DamageAmount);

	// Add to score
	if (AArenaShooterGameState* ArenaShooterGameState = GetWorld()->GetGameState<AArenaShooterGameState>())
	{
		ArenaShooterGameState->AddScore(DamageAmount);
	}

	// Check if the enemy is dead
	if (GetHealth() <= 0)
	{
		// Handle death
		Die();
	}

	// Return the amount of damage applied
	return DamageAmount;
}

void AEnemy::Attack()
{
	// Since this is the base class, don't have any attacks...
}

void AEnemy::Die()
{
	
	// Get rid of this and provide something more modular
	if (AArenaShooterGameMode* ArenaShooterGameMode = Cast<AArenaShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		ArenaShooterGameMode->KilledEnemy();
	}
	if (OnDeath.IsBound())
	{
		OnDeath.Execute();
	}
	Destroy();
}



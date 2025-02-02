
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Projectiles/AcceleratingProjectile.h"

void AAcceleratingProjectile::Shoot(FVector Velocity)
{
	ProjectileMovementComponent->Velocity = Velocity.GetSafeNormal() * 1.0f;
	ProjectileMovementComponent->MaxSpeed = Velocity.Length();
}

void AAcceleratingProjectile::IncreaseSpeed()
{
	// Increase the speed of the projectile by multiplying its magnitude by the multiplier
	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal() * ProjectileMovementComponent->Velocity.Length() * SpeedMultiplier;
	ProjectileMovementComponent->UpdateComponentVelocity();
}

void AAcceleratingProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(AcceleratingTimerHandle, this, &AAcceleratingProjectile::IncreaseSpeed, SpeedIncreaseDelay, true);
}

AAcceleratingProjectile::AAcceleratingProjectile()
{
	SpeedIncreaseDelay = 0.1f;
	SpeedMultiplier = 2.0f;
}

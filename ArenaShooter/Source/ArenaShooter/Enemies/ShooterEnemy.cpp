// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemy.h"
#include "EnemyController.h"

AShooterEnemy::AShooterEnemy()
{
	ProjectileType = AProjectile::StaticClass();
	ProjectileSpeed = 2000.0f;
	ProjectileOriginLocalOffset = FVector::Zero();
	MuzzleLateralOffset = 100.0f;
	MuzzleZOffset = 60.0f;
}

void AShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AShooterEnemy::Attack()
{
	if (ProjectileType) {
		if (UWorld* World = GetWorld()) {
			// Aim at the target is possible
			if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
			{
				if (AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(FName("Target"))))
				{
					FVector Origin = GetActorLocation() + ProjectileOriginLocalOffset;
					FVector TargetDir = (Target->GetActorLocation() - Origin).GetSafeNormal();
					FVector LateralDir = TargetDir * MuzzleLateralOffset;
					FTransform Transform = FTransform(TargetDir.Rotation(), Origin + LateralDir + FVector(0.0f, 0.0f, MuzzleZOffset));

					// Spawn the projectile at desired location
					AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(ProjectileType, Transform);
					if (Projectile)
					{
						// Setup settings of projectile
						Projectile->Shoot(TargetDir * ProjectileSpeed);
						Projectile->SetOwner(this);

						// Finalize
						Projectile->FinishSpawning(Transform);
					}
				}
			}
		}
	}
}

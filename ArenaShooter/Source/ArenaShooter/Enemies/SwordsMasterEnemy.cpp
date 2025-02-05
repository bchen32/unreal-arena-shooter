// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SwordsMasterEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ArenaShooterCharacter.h"
#include "Engine/DamageEvents.h"

ASwordsMasterEnemy::ASwordsMasterEnemy()
{
	LungePower = 1000.0f;
}

void ASwordsMasterEnemy::Attack()
{
	// Lunge attack
	Jump();

	// Lunge towards player
	if (UCapsuleComponent* Hitbox = GetCapsuleComponent())
	{
		if (AEnemyController* EnemyController = GetController<AEnemyController>())
		{
			if (UBlackboardComponent* Blackboard = EnemyController->GetBlackboardComponent())
			{
				if (AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(FName("Target"))))
				{
					FVector TargetDir = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

					GetCharacterMovement()->Velocity += TargetDir * LungePower;
				}
			}
		}
	}
}

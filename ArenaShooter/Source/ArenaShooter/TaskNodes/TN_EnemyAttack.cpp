// Fill out your copyright notice in the Description page of Project Settings.


#include "TN_EnemyAttack.h"
#include "../Enemies/EnemyController.h"

EBTNodeResult::Type UTN_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	if (AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComponent.GetAIOwner())) {
		EnemyController->Attack();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

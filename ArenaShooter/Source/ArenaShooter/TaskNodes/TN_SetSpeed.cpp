// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/TN_SetSpeed.h"
#include "TN_SetSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTN_SetSpeed::UTN_SetSpeed()
{
	NewSpeed = 300.0f;
}

EBTNodeResult::Type UTN_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	if (ACharacter* AICharacter = Cast<ACharacter>(OwnerComponent.GetAIOwner()->GetPawn()))
	{
		AICharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

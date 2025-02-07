// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/TN_SetSpeed.h"
#include "TN_SetSpeed.h"

UTN_SetSpeed::UTN_SetSpeed()
{
	NewSpeed = 300.0f;
}

EBTNodeResult::Type UTN_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	//if (ACharacter* AICharacter = Cast<ACharacter>(OwnerComponent.GetAIController()->GetPawn()))
	//{
	//	AICharacter->
	//}

	return EBTNodeResult::Succeeded;
}

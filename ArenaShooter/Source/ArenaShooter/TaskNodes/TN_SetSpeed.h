// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TN_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API UTN_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	float NewSpeed;
public:
	UTN_SetSpeed();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "TN_SideStep.generated.h"

/**
 * Takes in a EQS and alternates between moving left and right of the items in that query.
 */
UCLASS()
class ARENASHOOTER_API UTN_SideStep : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UBehaviorTreeComponent* OwnerComp;

protected:
	void OnSideStepQueryFinished(TSharedPtr<FEnvQueryResult> QueryResults);

	/** Must be set for the logic to alternate between side stepping left or right. */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector JustMovedLeftBlackboardKey;

	/** The player context */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetBlackboardKey;

	/** The outputted result */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ResultBlackboardKey;

	/** EQS for side stepping. */
	UPROPERTY(EditAnywhere)
	UEnvQuery* SideStepEQS;


public:
	UTN_SideStep();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory);
};

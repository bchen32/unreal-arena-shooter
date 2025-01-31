// Fill out your copyright notice in the Description page of Project Settings.


#include "TN_SetGameplayFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UTN_SetGameplayFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	if (UBlackboardComponent* BlackboardComponent = OwnerComponent.GetBlackboardComponent())
	{
		if (AAIController* AIController = OwnerComponent.GetAIOwner())
		{
			if (AActor* ActorFocus = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName)))
			{
				AIController->SetFocus(ActorFocus);
				return EBTNodeResult::Succeeded;
			}
			else
			{
				FVector LocationFocus = BlackboardComponent->GetValueAsVector(BlackboardKey.SelectedKeyName);
				if (LocationFocus.IsZero() == false) {
					AIController->SetFocalPoint(LocationFocus);
					return EBTNodeResult::Succeeded;
				}
				else {
					AIController->SetFocus(nullptr);
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

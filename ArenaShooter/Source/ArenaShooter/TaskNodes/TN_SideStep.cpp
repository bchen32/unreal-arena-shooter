// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/TN_SideStep.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UTN_SideStep::OnSideStepQueryFinished(TSharedPtr<FEnvQueryResult> QueryResults)
{
	if (QueryResults.IsValid() == false)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		return;
	}

	TArray<FVector> ItemLocations;
	QueryResults->GetAllAsLocations(ItemLocations);
	if (ItemLocations.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("A"));
		UBlackboardComponent* BlackboardComp = OwnerComp->GetBlackboardComponent();
		AActor* AIActor = OwnerComp->GetAIOwner()->GetPawn();

		// Get the right direction from the actor to the target
		AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetBlackboardKey.SelectedKeyName));
		FVector ToTarget = (Target->GetActorLocation() - AIActor->GetActorLocation()).GetSafeNormal();
		FVector Up = AIActor->GetActorUpVector();
		FVector Right = Up.Cross(ToTarget).GetSafeNormal();

		for (FVector& ItemLocation : ItemLocations)
		{
			// Direction to item for dot product comparison
			FVector ToItem = (ItemLocation - AIActor->GetActorLocation()).GetSafeNormal();
			
			// Direction of the item
			float DotProduct = Right.Dot(ToItem);
			
			// Just moved left?
			if (BlackboardComp->GetValueAsBool(JustMovedLeftBlackboardKey.SelectedKeyName))
			{
				if (DotProduct > 0)
				{
					// Move right now
					BlackboardComp->SetValueAsVector(ResultBlackboardKey.SelectedKeyName, ItemLocation);
					BlackboardComp->SetValueAsBool(JustMovedLeftBlackboardKey.SelectedKeyName, false);
					FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
					return;
				}
			}
			else
			{
				if (DotProduct < 0)
				{
					// Move left now
					BlackboardComp->SetValueAsVector(ResultBlackboardKey.SelectedKeyName, ItemLocation);
					BlackboardComp->SetValueAsBool(JustMovedLeftBlackboardKey.SelectedKeyName, true);
					FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
					return;
				}
			}
		}
	}
	
	FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
}

UTN_SideStep::UTN_SideStep()
{
	SideStepEQS = nullptr;

	bCreateNodeInstance = true;

	OwnerComp = nullptr;

	JustMovedLeftBlackboardKey.AddBoolFilter(this, FName("JustMovedLeftKey"));
	ResultBlackboardKey.AddVectorFilter(this, FName("ResultKey"));
	TargetBlackboardKey.AddObjectFilter(this, FName("TargetBlackboardKey"), AActor::StaticClass());
}

EBTNodeResult::Type UTN_SideStep::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	// Check to make sure that the blackboard key is a 
	if (JustMovedLeftBlackboardKey.IsNone()) return EBTNodeResult::Failed;
	if (ResultBlackboardKey.IsNone()) return EBTNodeResult::Failed;
	if (TargetBlackboardKey.IsNone()) return EBTNodeResult::Failed;
	if (SideStepEQS == nullptr) return EBTNodeResult::Failed;
	if (OwnerComponent.GetOwner() == nullptr) return EBTNodeResult::Failed;
	if (OwnerComponent.GetBlackboardComponent() == nullptr) return EBTNodeResult::Failed;
	OwnerComp = &OwnerComponent;

	// Run the query
	FEnvQueryRequest QueryRequest = FEnvQueryRequest(SideStepEQS, OwnerComponent.GetAIOwner()->GetPawn());
	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UTN_SideStep::OnSideStepQueryFinished);

	return EBTNodeResult::InProgress;
}
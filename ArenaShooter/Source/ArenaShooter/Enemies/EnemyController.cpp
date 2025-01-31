// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

AEnemyController::AEnemyController()
{
	// AI
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemyController::Attack()
{
	if (AEnemy* Enemy = Cast<AEnemy>(GetPawn())) {
		Enemy->Attack();
	}
	else {
		Debug(TEXT("Could not make Enemy attack."));
	}
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// On spawn, always know who to target (the player)
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(FName("Target"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemy* Enemy = Cast<AEnemy>(InPawn))
	{
		// Setup blackboard (memory of the behavior tree)
		UBehaviorTree* EnemysBehaviorTree = Enemy->GetBehaviorTree();
		if (EnemysBehaviorTree && EnemysBehaviorTree->BlackboardAsset)
		{
			// Initialize blackboard with the initial values
			bool SuccessfulBlackboardInit = true;
			SuccessfulBlackboardInit = BlackboardComponent->InitializeBlackboard(*(EnemysBehaviorTree->BlackboardAsset));

			// Activates the behavior tree immediately if the data made it through
			if (SuccessfulBlackboardInit)
			{
				BehaviorTreeComponent->StartTree(*(EnemysBehaviorTree));
			}
			else
			{
				Debug(TEXT("Failed to initialize blackboard. Interrupting behavior tree start."), FColor::Red);
			}
		}
		else
		{
			// Error messages
			if (EnemysBehaviorTree == nullptr) Debug(TEXT("Failed to initialize the blackboard because Enemy's behavior tree was null."), FColor::Red);
			else Debug(TEXT("Failed to initialize blackboard becauase Enemy's behavior tree's blackboard asset was null."), FColor::Red);
		}
	}
	else
	{
		// Error message
		Debug(TEXT("Failed to possess because it was not of type Enemy."), FColor::Red);
	}
}

void AEnemyController::Debug(const FString& Log, FColor Color)
{
	if (GEngine) {
		FString PrefixedString = FString::Printf(TEXT("[%s] %s"), *GetActorNameOrLabel(), *Log);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, Color, PrefixedString);
	}
}


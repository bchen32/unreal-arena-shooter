// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class ARENASHOOTER_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
private:
	// AI Components
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* BlackboardComponent;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void Debug(const FString& Log, FColor Color = FColor::Yellow);

public:
	AEnemyController();

	UFUNCTION(BlueprintCallable)
	void Attack();
};

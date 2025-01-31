// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, OldHealth, float, NewHealth);
DECLARE_DELEGATE(FOnDeath);

UCLASS(BlueprintType, Blueprintable)
class ARENASHOOTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	float Health;

protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;

	/** Event for when the health changes */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

public:	
	/** Event for when enemy dies */
	FOnDeath OnDeath;

	// Sets default values for this character's properties
	AEnemy();

	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	float GetHealth() const { return Health; }
	void SetHealth(float NewHealth);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attack();
	virtual void Die();

};

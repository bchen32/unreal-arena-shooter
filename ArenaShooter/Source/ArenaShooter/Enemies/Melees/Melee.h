// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Melee.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Melee Hitbox"), meta = (BlueprintSpawnableComponent))
class ARENASHOOTER_API UMelee : public UBoxComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	float Damage;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
public:
	UMelee();

	void Activate();
	void Disable();
};

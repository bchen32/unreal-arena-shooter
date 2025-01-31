// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class ARENASHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
private:	
	UPROPERTY(EditAnywhere)
	USphereComponent* HitboxComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	// Whoever shot the projectile.
	AActor* Owner = nullptr;

	FTimerHandle ExpirationTimerHandle = FTimerHandle();

protected:
	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float Lifetime;

	virtual void BeginPlay() override;

	// Effect to perform when the projectile hits something
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Actions
	virtual void Shoot(FVector Velocity);
	virtual void Expire();

	// Getters
	float GetDamage() const { return Damage; }
	void SetDamage(float NewDamage) { Damage = NewDamage; }
	AActor* GetOwner() const { return Owner; }
	void SetOwner(AActor* NewOwner) { Owner = NewOwner; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogWeapons, Log, All);

UCLASS()
class ARENASHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

private:
	int32 CurrAmmo;
	float TimeSinceLastShot;
	bool bIsShooting;
	bool bIsReloading;
	bool bIsActive;
	FTimerHandle PulloutTimerHandle;
	FTimerHandle ReloadTimerHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* GunMesh;

	// Sound effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* KillSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* EquipSound;

	// Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
	UAnimMontage* ShootAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
	UAnimMontage* ReloadAnim;

	// Gameplay stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	float ReloadCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	float ShootCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	float PulloutTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	bool bIsSemiAuto;

public:
	ABaseWeapon();

	virtual void Tick(float DeltaTime) override;

	virtual void StartShoot();

	virtual void EndShoot();

	virtual void StartReload();

	virtual void Enable();

	virtual void Disable();

	USkeletalMeshComponent* GetGunMesh() const { return GunMesh; }

protected:
	virtual void CompletePullout();

	virtual void CompleteReload();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

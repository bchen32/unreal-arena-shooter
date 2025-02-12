// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AArenaShooterCharacter;
class AArenaShooterPlayerController;
class UAudioComponent;
class USkeletalMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogWeapons, Log, All);

UCLASS()
class ARENASHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* GunMesh;

	// Sound effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* MetaSound; // handles all sfx logic

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
	float EquipTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	bool bIsSemiAuto;

	UPROPERTY()
	AArenaShooterCharacter* OwningCharacter;

	UPROPERTY()
	AArenaShooterPlayerController* OwningController;

	int32 CurrAmmo;
	float TimeSinceLastShot;
	bool bIsShooting;
	bool bIsReloading;
	bool bIsActive;
	FTimerHandle EquipTimerHandle;
	FTimerHandle ReloadTimerHandle;

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void StartShoot();

	virtual void EndShoot();

	virtual void StartReload();

	virtual void Enable();

	virtual void Disable();

	virtual void OnKill();

	void SetArenaShooterOwner(AArenaShooterCharacter* Character);

	USkeletalMeshComponent* GetGunMesh() const { return GunMesh; }

protected:
	virtual void CompleteEquip();

	virtual void CompleteReload();

	virtual void PlayAnim(UAnimMontage* Anim);

	virtual void SetFloatMetaSound(const FName& ParamName, float NewVal);

	virtual void TriggerMetaSound(const FName& TriggerName);

	virtual void UpdateHUD();

};

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
	USoundBase* ShootMetaSound; // handles shoot, echo, hit, and critical hit

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
	float EquipTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	bool bIsSemiAuto;

	int32 CurrAmmo;
	float TimeSinceLastShot;
	bool bIsShooting;
	bool bIsReloading;
	bool bIsActive;
	UAudioComponent* EquipAudioComponent;
	UAudioComponent* ReloadAudioComponent;
	UAudioComponent* ShootAudioComponent;
	FTimerHandle EquipTimerHandle;
	FTimerHandle ReloadTimerHandle;
	AArenaShooterCharacter* OwningCharacter;
	AArenaShooterPlayerController* OwningController;

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

	virtual void UpdateHUD();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class AArenaShooterCharacter;
class APlayerController;
class ABaseWeapon;

DECLARE_LOG_CATEGORY_EXTERN(LogEquipment, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchBackwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SelectAction0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SelectAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SelectAction2;

	// Weapons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

	TArray<ABaseWeapon*> Weapons;
	int32 CurrWeaponIdx;

	// Character
	AArenaShooterCharacter* OwningCharacter;
	APlayerController* OwningController;

public:
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

private:
	bool ValidWeapon(int32 idx);

	void StartShoot();

	void EndShoot();

	void StartReload();

	void SwitchForward();

	void SwitchBackward();

	void Switch0();

	void Switch1();

	void Switch2();

	void Switch(int32 idx);

	void AttachCurrMesh();

	void OnKill();

};

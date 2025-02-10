// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class AArenaShooterCharacter;
class APlayerController;
class AWeapon;

DECLARE_LOG_CATEGORY_EXTERN(LogEquipment, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SwitchForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SwitchBackwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SelectAction0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SelectAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SelectAction2;

	// Weapons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
	int32 StartingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	TArray<AWeapon*> Weapons;
	int32 CurrWeaponIdx;
	AArenaShooterCharacter* OwningCharacter;
	APlayerController* OwningController;

public:
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

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

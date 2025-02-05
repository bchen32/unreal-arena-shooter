// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/EquipmentComponent.h"
#include "ArenaShooterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapons/BaseWeapon.h"

DEFINE_LOG_CATEGORY(LogEquipment);

UEquipmentComponent::UEquipmentComponent()
{
	CurrWeaponIdx = 0;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	// Spawn guns
	for (TSubclassOf<ABaseWeapon> WeaponClass : WeaponClasses)
	{
		if (UWorld* World = GetWorld())
		{
			ABaseWeapon* NewWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass);
			if (NewWeapon)
			{
				Weapons.Add(NewWeapon);
			}
		}
	}
	if (ValidWeapon(CurrWeaponIdx))
	{
		Weapons[CurrWeaponIdx]->Enable();
		AttachCurrMesh();
	}
	// Bind input
	if (AActor* OwningActor = GetOwner())
	{
		if (ACharacter* Character = Cast<ACharacter>(OwningActor))
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
				{
					EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &UEquipmentComponent::StartShoot);
					EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &UEquipmentComponent::EndShoot);

					EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UEquipmentComponent::StartReload);

					EnhancedInputComponent->BindAction(SwitchForwardAction, ETriggerEvent::Started, this, &UEquipmentComponent::SwitchForward);
					EnhancedInputComponent->BindAction(SwitchBackwardAction, ETriggerEvent::Started, this, &UEquipmentComponent::SwitchBackward);
					EnhancedInputComponent->BindAction(SelectAction0, ETriggerEvent::Started, this, &UEquipmentComponent::Switch0);
					EnhancedInputComponent->BindAction(SelectAction1, ETriggerEvent::Started, this, &UEquipmentComponent::Switch1);
					EnhancedInputComponent->BindAction(SelectAction2, ETriggerEvent::Started, this, &UEquipmentComponent::Switch2);
				}
			}
		}
	}
}

bool UEquipmentComponent::ValidWeapon(int32 idx)
{
	return idx >= 0 && idx < Weapons.Num();
}

void UEquipmentComponent::StartShoot()
{
	if (ValidWeapon(CurrWeaponIdx))
	{
		Weapons[CurrWeaponIdx]->StartShoot();
	}
}

void UEquipmentComponent::EndShoot()
{
	if (ValidWeapon(CurrWeaponIdx))
	{
		Weapons[CurrWeaponIdx]->EndShoot();
	}
}

void UEquipmentComponent::StartReload()
{
	if (ValidWeapon(CurrWeaponIdx))
	{
		Weapons[CurrWeaponIdx]->StartReload();
	}
}

// Switch methods are repetitive but not sure how to improve
void UEquipmentComponent::SwitchForward()
{
	int32 NewIdx = (CurrWeaponIdx + 1) % Weapons.Num();
	Switch(NewIdx);
}

void UEquipmentComponent::SwitchBackward()
{
	int32 NewIdx = (CurrWeaponIdx + Weapons.Num() - 1) % Weapons.Num();
	Switch(NewIdx);
}

void UEquipmentComponent::Switch0()
{
	Switch(0);
}

void UEquipmentComponent::Switch1()
{
	Switch(1);
}

void UEquipmentComponent::Switch2()
{
	Switch(2);
}

void UEquipmentComponent::Switch(int32 idx)
{
	UE_LOG(LogEquipment, Log, TEXT("Switch: %d"), idx);
	if (CurrWeaponIdx != idx && ValidWeapon(CurrWeaponIdx) && ValidWeapon(idx))
	{
		// Disable and detach prev weapon
		Weapons[CurrWeaponIdx]->Disable();
		if (USkeletalMeshComponent* OldGunMesh = Weapons[CurrWeaponIdx]->GetGunMesh())
		{
			OldGunMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}

		CurrWeaponIdx = idx;

		// Enable and attach new weapon
		Weapons[CurrWeaponIdx]->Enable();
		AttachCurrMesh();
	}
}

void UEquipmentComponent::AttachCurrMesh()
{
	if (AActor* OwningActor = GetOwner())
	{
		if (AArenaShooterCharacter* ArenaCharacter = Cast<AArenaShooterCharacter>(OwningActor))
		{
			if (USkeletalMeshComponent* CharacterMesh = ArenaCharacter->GetMesh1P())
			{
				if (USkeletalMeshComponent* NewGunMesh = Weapons[CurrWeaponIdx]->GetGunMesh())
				{
					FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
					NewGunMesh->AttachToComponent(CharacterMesh, AttachmentRules, FName(TEXT("GripPoint")));
					UE_LOG(LogEquipment, Log, TEXT("Attach"));
				}
			}
		}
	}
}

void UEquipmentComponent::OnKill()
{

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/EquipmentComponent.h"
#include "ArenaShooterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/Weapon.h"

DEFINE_LOG_CATEGORY(LogEquipment);

UEquipmentComponent::UEquipmentComponent()
{
	StartingWeapon = 0;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	// Bind input and get parent
	if (AActor* OwningActor = GetOwner())
	{
		OwningCharacter = Cast<AArenaShooterCharacter>(OwningActor);
		if (OwningCharacter)
		{
			OwningController = Cast<APlayerController>(OwningCharacter->GetController());
			if (OwningController)
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwningCharacter->InputComponent))
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
			else
			{
				UE_LOG(LogEquipment, Error, TEXT("Failed to initialize EquipmentComponent, no controller"));
			}
		}
		else
		{
			UE_LOG(LogEquipment, Error, TEXT("Failed to initialize EquipmentComponent, no character"));
		}
	}
	else
	{
		UE_LOG(LogEquipment, Error, TEXT("Failed to initialize EquipmentComponent, no owner"));
	}
	// Spawn guns
	for (TSubclassOf<AWeapon> WeaponClass : WeaponClasses)
	{
		if (UWorld* World = GetWorld())
		{
			AWeapon* NewWeapon = World->SpawnActor<AWeapon>(WeaponClass);
			if (NewWeapon)
			{
				Weapons.Add(NewWeapon);
				NewWeapon->SetArenaShooterOwner(OwningCharacter);
			}
		}
	}
	// Activate starting weapon
	CurrWeaponIdx = StartingWeapon;
	if (ValidWeapon(CurrWeaponIdx))
	{
		Weapons[CurrWeaponIdx]->Enable();
		AttachCurrMesh();
	}
	else
	{
		UE_LOG(LogEquipment, Log, TEXT("No starting weapon equipped"));
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
	if (ValidWeapon(CurrWeaponIdx) && ValidWeapon(idx))
	{
		if (CurrWeaponIdx != idx)
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
	else
	{
		UE_LOG(LogEquipment, Warning, TEXT("Cannot switch to: %d"), idx);
	}
}

void UEquipmentComponent::AttachCurrMesh()
{
	if (OwningCharacter)
	{
		if (USkeletalMeshComponent* CharacterMesh = OwningCharacter->GetMesh1P())
		{
			if (USkeletalMeshComponent* NewGunMesh = Weapons[CurrWeaponIdx]->GetGunMesh())
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
				NewGunMesh->AttachToComponent(CharacterMesh, AttachmentRules, FName(TEXT("GripPoint")));
			}
		}
	}
	else
	{
		UE_LOG(LogEquipment, Warning, TEXT("No owning character to attach mesh"));
	}
}

void UEquipmentComponent::OnKill() // hook this up once there's an enemy manager
{

}

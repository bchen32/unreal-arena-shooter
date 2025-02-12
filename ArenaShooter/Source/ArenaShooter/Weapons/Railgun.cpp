// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Railgun.h"

ARailgun::ARailgun()
    : Super()
{

	Damage = 30.0f;
	MaxAmmo = 1;
	ReloadCooldown = 8.0f;
	bIsSemiAuto = true;
}

void ARailgun::StartReload() // same as AWeapon but reset recharging sound start time
{
    if (bIsActive && !bIsReloading && CurrAmmo < MaxAmmo)
    {
        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ARailgun::CompleteReload, ReloadCooldown, false);
        bIsReloading = true;

        PlayAnim(ReloadAnim);

        // Set start param
        SetFloatMetaSound(FName("ReloadStart"), 0.0f);
        TriggerMetaSound(FName("Reload"));
    }
}

void ARailgun::Enable() // same as AWeapon but resume recharging sound and don't clear reload timer
{
    SetActorHiddenInGame(false);
    GetWorld()->GetTimerManager().SetTimer(EquipTimerHandle, this, &ARailgun::CompleteEquip, EquipTime, false);
    // Play pull out animation TODO
    if (GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
    {
        float TimeLeft = GetWorld()->GetTimerManager().GetTimerRemaining(ReloadTimerHandle);
        if (TimeLeft > 0)
        {
            // Resume reload based on timer
            SetFloatMetaSound(FName("ReloadStart"), ReloadCooldown - TimeLeft);
            TriggerMetaSound(FName("Reload"));
        }
    }
    TriggerMetaSound(FName("Equip"));
    UpdateHUD();
}

void ARailgun::Disable() // same as AWeapon but don't clear reload timer
{
    SetActorHiddenInGame(true);
    GetWorld()->GetTimerManager().ClearTimer(EquipTimerHandle);
    bIsActive = false;
    bIsShooting = false;
    PrimaryActorTick.bCanEverTick = false;
    TriggerMetaSound(FName("Switch"));
}

void ARailgun::OnKill() // railgun half recharges on kill
{
    TriggerMetaSound(FName("Kill"));
    if (GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
    {
        float TimeLeft = GetWorld()->GetTimerManager().GetTimerRemaining(ReloadTimerHandle);
        if (TimeLeft > 0)
        {
            float NewTimeLeft = TimeLeft - (ReloadCooldown / 2.0f);
            if (NewTimeLeft <= 0)
            {
                GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
                CompleteReload();
            }
            else
            {
                GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ARailgun::CompleteReload, NewTimeLeft, false);
            }
        }
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ARailgun::CompleteReload, ReloadCooldown / 2.0f, false);
        bIsReloading = true;
    }
}

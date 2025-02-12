// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "ArenaShooterCharacter.h"
#include "ArenaShooterPlayerController.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemies/Enemy.h"
#include "Enemies/Accessories/WeakSpotComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"

DEFINE_LOG_CATEGORY(LogWeapons);

AWeapon::AWeapon()
{
	// Mesh component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	RootComponent = GunMesh;

	// Audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootAudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bAllowSpatialization = false; // no point spatializing player sound, just play as 2D stereo
	AudioComponent->SetupAttachment(RootComponent);

	Damage = 1.0f;
	MaxAmmo = 80;
	ReloadCooldown = 2.5f;
	ShootCooldown = 0.05f;
	EquipTime = 0.25f;
	bIsSemiAuto = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(MetaSound) && IsValid(AudioComponent))
	{
		AudioComponent->SetSound(MetaSound);
		AudioComponent->Play();
	}
	CurrAmmo = MaxAmmo;
	TimeSinceLastShot = 0.0f;
	bIsShooting = false;
	bIsReloading = false;
	bIsActive = false;
	SetActorHiddenInGame(true);
}

void AWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(EquipTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	TriggerMetaSound(FName("Stop"));
	Super::EndPlay(EndPlayReason);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastShot += DeltaTime;
	if (bIsActive && !bIsReloading)
	{
		if (CurrAmmo <= 0)
		{
			StartReload();
			return;
		}

		if (bIsShooting)
		{
			if (TimeSinceLastShot >= ShootCooldown)
			{
				TimeSinceLastShot = 0.0f;
				--CurrAmmo;
				// Line trace
				if (!IsValid(OwningController) || !IsValid(OwningCharacter))
				{
					UE_LOG(LogWeapons, Error, TEXT("Can't linetrace, no owning Controller or Character"));
					return;
				}
				FVector CameraLocation;
				FRotator CameraRotation;
				OwningController->GetPlayerViewPoint(CameraLocation, CameraRotation);

				FVector Start = CameraLocation;
				FVector End = Start + (CameraRotation.Vector() * 10000.0f);

				FCollisionQueryParams CollisionParams;
				CollisionParams.bTraceComplex = true;

				FHitResult HitResult;
				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, CollisionParams); // player attack channel
				if (bHit)
				{
					AActor* HitActor = HitResult.GetActor();
					if (IsValid(HitActor))
					{
						AEnemy* Enemy = Cast<AEnemy>(HitActor);
						if (IsValid(Enemy))
						{
							Enemy->OnDeath.BindUObject(this, &AWeapon::OnKill);

							// Apply critical damage modifiers
							float ResultantDamage = Damage;
							UWeakSpotComponent* WeakSpot = Cast<UWeakSpotComponent>(HitResult.GetComponent());
							if (IsValid(WeakSpot))
							{
								ResultantDamage = WeakSpot->ApplyDamageModifier(Damage);
								TriggerMetaSound(FName("Critical"));
							}
							else
							{
								TriggerMetaSound(FName("Hit"));
							}
							// Health regen on damage, can improve
							OwningCharacter->CurrHealth = FMath::Min(OwningCharacter->MaxHealth, OwningCharacter->CurrHealth + (ResultantDamage / 5.0f));
							UGameplayStatics::ApplyPointDamage(HitActor, ResultantDamage, HitResult.ImpactPoint, HitResult, OwningController, OwningCharacter, nullptr);
						}
					}
					// Play impact vfx
				}
				PlayAnim(ShootAnim);

				TriggerMetaSound(FName("Shoot"));

				// Play muzzle flash

				// Play bullet tracer

				// Update HUD
				UpdateHUD();
			}
		}
	}
	if (bIsSemiAuto)
	{
		/* Runs regardless of whether shot
		was fired to prevent shot queuing */
		bIsShooting = false;
	}
}

void AWeapon::StartShoot()
{
	bIsShooting = true;
}

void AWeapon::EndShoot()
{
	bIsShooting = false;
}

void AWeapon::StartReload()
{
	if (bIsActive && !bIsReloading && CurrAmmo < MaxAmmo)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::CompleteReload, ReloadCooldown, false);
		bIsReloading = true;

		PlayAnim(ReloadAnim);

		TriggerMetaSound(FName("Reload"));
	}
}

void AWeapon::Enable()
{
	SetActorHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(EquipTimerHandle, this, &AWeapon::CompleteEquip, EquipTime, false);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle); // equipping weapon resets reload
	bIsReloading = false;
	// Play pull out animation TODO
	TriggerMetaSound(FName("Equip"));
	UpdateHUD();
}

void AWeapon::Disable()
{
	SetActorHiddenInGame(true);
	GetWorld()->GetTimerManager().ClearTimer(EquipTimerHandle);
	if (GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
	{
		if (CurrAmmo == 0) // pocket reload if empty
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::CompleteReload, ReloadCooldown, false);
			bIsReloading = true;
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
			bIsReloading = false;
		}
	}
	bIsActive = false;
	bIsShooting = false;
	PrimaryActorTick.bCanEverTick = false;
	TriggerMetaSound(FName("Switch"));
}

void AWeapon::OnKill()
{
	TriggerMetaSound(FName("Kill"));
	CompleteReload();
}

void AWeapon::SetArenaShooterOwner(AArenaShooterCharacter* Character)
{
	OwningCharacter = Character;
	OwningController = Cast<AArenaShooterPlayerController>(Character->GetController());
}

void AWeapon::CompleteEquip()
{
	bIsActive = true;
	PrimaryActorTick.bCanEverTick = true;
	if (CurrAmmo > 0)
	{
		TriggerMetaSound(FName("Idle"));
	}
}

void AWeapon::CompleteReload()
{
	CurrAmmo = MaxAmmo;
	bIsReloading = false;
	if (bIsActive)
	{
		TriggerMetaSound(FName("Idle"));
		UpdateHUD();
	}
}

void AWeapon::PlayAnim(UAnimMontage* Anim)
{
	if (!IsValid(OwningCharacter))
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't play anim, no owning character"));
		return;
	}
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh1P()->GetAnimInstance();
	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't play anim, owning character no AnimInstance"));
		return;
	}
	if (IsValid(Anim))
	{
		AnimInstance->Montage_Play(Anim, 1.0f);
	}
}

void AWeapon::SetFloatMetaSound(const FName& ParamName, float NewVal)
{
	if (!IsValid(AudioComponent))
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't set MetaSound param, invalid AudioComponent"));
		return;
	}
	AudioComponent->SetFloatParameter(ParamName, NewVal);
}

void AWeapon::TriggerMetaSound(const FName& TriggerName)
{
	if (!IsValid(AudioComponent))
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't play sound, invalid AudioComponent"));
		return;
	}
	AudioComponent->SetTriggerParameter(TriggerName);
}

void AWeapon::UpdateHUD()
{
	if (!IsValid(OwningController) || !IsValid(OwningController->HUD)) // refactor HUD, should probably use a getter
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't update HUD, no owning Controller with HUD"));
		return;
	}
	OwningController->HUD->SetAmmo(CurrAmmo, MaxAmmo);
}

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

	// Audio components
	EquipAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EquipAudioComponent"));
	EquipAudioComponent->bAutoActivate = false;
	EquipAudioComponent->SetupAttachment(RootComponent);
	ReloadAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ReloadAudioComponent"));
	ReloadAudioComponent->bAutoActivate = false;
	ReloadAudioComponent->SetupAttachment(RootComponent);
	ShootAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootAudioComponent"));
	ShootAudioComponent->bAutoActivate = false;
	ShootAudioComponent->SetupAttachment(RootComponent);

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
	if (EquipSound)
	{
		EquipAudioComponent->SetSound(EquipSound);
	}
	if (ReloadSound)
	{
		ReloadAudioComponent->SetSound(ReloadSound);
	}
	if (ShootMetaSound)
	{
		ShootAudioComponent->SetSound(ShootMetaSound);
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
				if (!OwningController)
				{
					UE_LOG(LogWeapons, Error, TEXT("Can't linetrace, no owning controller"));
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
					if (HitActor)
					{
						if (AEnemy* Enemy = Cast<AEnemy>(HitActor))
						{
							Enemy->OnDeath.BindUObject(this, &AWeapon::OnKill);

							// Apply critical damage modifiers
							float ResultantDamage = Damage;
							if (UWeakSpotComponent* WeakSpot = Cast<UWeakSpotComponent>(HitResult.GetComponent()))
							{
								ResultantDamage = WeakSpot->ApplyDamageModifier(Damage);
								if (ShootAudioComponent)
								{
									ShootAudioComponent->SetTriggerParameter(FName("Critical"));
								}
							}
							else
							{
								ShootAudioComponent->SetTriggerParameter(FName("Hit"));
							}

							UGameplayStatics::ApplyPointDamage(HitActor, ResultantDamage, HitResult.ImpactPoint, HitResult, OwningController, OwningCharacter, nullptr);
						}
					}
					// Play impact vfx
				}
				if (ShootAnim)
				{
					PlayAnim(ShootAnim);
				}

				if (ShootAudioComponent)
				{
					ShootAudioComponent->SetTriggerParameter(FName("Shoot"));
				}

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

		if (ReloadAnim)
		{
			PlayAnim(ReloadAnim);
		}

		if (ReloadAudioComponent)
		{
			ReloadAudioComponent->Play();
		}
	}
}

void AWeapon::Enable()
{
	SetActorHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(EquipTimerHandle, this, &AWeapon::CompleteEquip, EquipTime, false);
	UpdateHUD();
	// Play pull out animation TODO

	if (EquipAudioComponent)
	{
		EquipAudioComponent->Play();
	}
	if (ShootAudioComponent)
	{
		ShootAudioComponent->Play(); // just activates metasound, doesn't trigger any sound
	}
}

void AWeapon::Disable()
{
	SetActorHiddenInGame(true);
	GetWorld()->GetTimerManager().ClearTimer(EquipTimerHandle);
	bIsActive = false;
	bIsShooting = false;
	PrimaryActorTick.bCanEverTick = false;
	if (EquipAudioComponent && EquipAudioComponent->IsPlaying())
	{
		EquipAudioComponent->Stop();
	}
	if (ReloadAudioComponent && ReloadAudioComponent->IsPlaying())
	{
		ReloadAudioComponent->Stop();
	}
	if (ShootAudioComponent && ShootAudioComponent->IsPlaying())
	{
		ShootAudioComponent->SetTriggerParameter(FName("Stop")); // metasound has a 1s delay on stop to allow last gunshot to finish
	}
}

void AWeapon::OnKill()
{
	if (KillSound)
	{
		UGameplayStatics::PlaySound2D(this, KillSound);
	}
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
}

void AWeapon::CompleteReload()
{
	CurrAmmo = MaxAmmo;
	bIsReloading = false;
	if (bIsActive)
	{
		UpdateHUD();
	}
}

void AWeapon::PlayAnim(UAnimMontage* Anim)
{
	if (!OwningCharacter)
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't play anim, no owning character"));
		return;
	}
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh1P()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Anim, 1.0f);
	}
}

void AWeapon::UpdateHUD()
{
	if (!OwningController || !OwningController->HUD) // refactor HUD, should probably use a getter
	{
		UE_LOG(LogWeapons, Error, TEXT("Can't update HUD, no owning controller with HUD"));
		return;
	}
	OwningController->HUD->SetAmmo(CurrAmmo, MaxAmmo);
}

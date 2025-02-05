// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogWeapons);

ABaseWeapon::ABaseWeapon()
{
	// Mesh component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	RootComponent = GunMesh;

	Damage = 1.0f;
	MaxAmmo = 80;
	ReloadCooldown = 2.5f;
	ShootCooldown = 0.05f;
	PulloutTime = 0.25f;
	bIsSemiAuto = false;

	PrimaryActorTick.bCanEverTick = true;
}

void ABaseWeapon::Tick(float DeltaTime)
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
				UE_LOG(LogWeapons, Log, TEXT("Fire: %d"), CurrAmmo);
				// Line trace
				FHitResult HitResult;
				bool bHit = false;
				if (bHit)
				{
					AActor* HitActor = HitResult.GetActor();
					if (HitActor)
					{
						// Apply damage
					}

					// Play bullet impact effect and spawn decal
				}
				TimeSinceLastShot = 0.0f;
				--CurrAmmo;

				// Play shoot sound

				// Play animation
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

void ABaseWeapon::StartShoot()
{
	bIsShooting = true;
}

void ABaseWeapon::EndShoot()
{
	bIsShooting = false;
}

void ABaseWeapon::StartReload()
{
	if (bIsActive && !bIsReloading && CurrAmmo < MaxAmmo)
	{
		// Play reload sound
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ABaseWeapon::CompleteReload, ReloadCooldown, false);
		bIsReloading = true;
		UE_LOG(LogWeapons, Log, TEXT("Start reload"));
	}
}

void ABaseWeapon::Enable()
{
	SetActorHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(PulloutTimerHandle, this, &ABaseWeapon::CompletePullout, PulloutTime, false);
	// Play pull out animation
}

void ABaseWeapon::Disable()
{
	SetActorHiddenInGame(true);
	GetWorld()->GetTimerManager().ClearTimer(PulloutTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	bIsActive = false;
}

void ABaseWeapon::CompletePullout()
{
	bIsActive = true;
	UE_LOG(LogWeapons, Log, TEXT("Complete pullout"));
}

void ABaseWeapon::CompleteReload()
{
	CurrAmmo = MaxAmmo;
	bIsReloading = false;
	UE_LOG(LogWeapons, Log, TEXT("Complete reload"));
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrAmmo = MaxAmmo;
	TimeSinceLastShot = 0.0f;
	bIsShooting = false;
	bIsReloading = false;
	bIsActive = false;
	SetActorHiddenInGame(true);
}

void ABaseWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(PulloutTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	Super::EndPlay(EndPlayReason);
}

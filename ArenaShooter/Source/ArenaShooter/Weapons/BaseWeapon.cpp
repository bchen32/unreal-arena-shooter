// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooterCharacter.h"
#include "ArenaShooterPlayerController.h"
#include "Weapons/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemies/Enemy.h"
#include "Enemies/Accessories/WeakSpotComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"

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
			UE_LOG(LogWeapons, Log, TEXT("Empty reload"));
			StartReload();
			return;
		}

		if (bIsShooting)
		{
			if (TimeSinceLastShot >= ShootCooldown)
			{
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
				CollisionParams.AddIgnoredActor(OwningCharacter);
				//CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the rifle itself
				CollisionParams.bTraceComplex = true;       // Use complex collision

				FHitResult HitResult;
				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, CollisionParams);
				if (bHit)
				{
					AActor* HitActor = HitResult.GetActor();
					if (HitActor)
					{
						AEnemy* Enemy = Cast<AEnemy>(HitActor);
						if (Enemy)
						{
							// Bind the weapon’s function to the enemy’s delegate
							Enemy->OnDeath.BindUObject(this, &ABaseWeapon::OnKill);
						}

						// Apply damage modifiers
						float ResultantDamage = Damage;
						if (UWeakSpotComponent* WeakSpot = Cast<UWeakSpotComponent>(HitResult.GetComponent()))
						{
							ResultantDamage = WeakSpot->ApplyDamageModifier(Damage);
						}

						// Damage the hit
						UGameplayStatics::ApplyPointDamage(HitActor, ResultantDamage, HitResult.ImpactPoint, HitResult, OwningController, OwningCharacter, nullptr);
					}

					// Play bullet impact effect and spawn decal
				}
				TimeSinceLastShot = 0.0f;
				--CurrAmmo;

				// Update HUD
				UpdateHUD();

				// Play shoot sound
				if (ShootSound)
				{
					UGameplayStatics::PlaySound2D(this, ShootSound);
				}

				// Play animation
				if (ShootAnim)
				{
					PlayAnim(ShootAnim);
				}
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
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ABaseWeapon::CompleteReload, ReloadCooldown, false);
		bIsReloading = true;

		// Play sound
		if (ReloadSound)
		{
			UGameplayStatics::PlaySound2D(this, ReloadSound);
		}

		// Play anim
		if (ReloadAnim)
		{
			PlayAnim(ReloadAnim);
		}
	}
}

void ABaseWeapon::Enable()
{
	SetActorHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(PulloutTimerHandle, this, &ABaseWeapon::CompletePullout, PulloutTime, false);
	UpdateHUD();
	// Play pull out animation
}

void ABaseWeapon::Disable()
{
	SetActorHiddenInGame(true);
	GetWorld()->GetTimerManager().ClearTimer(PulloutTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	bIsActive = false;
	bIsReloading = false;
	bIsShooting = false;
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseWeapon::OnKill()
{
	if (KillSound)
	{
		UGameplayStatics::PlaySound2D(this, KillSound);
	}
	CompleteReload();
}

void ABaseWeapon::SetArenaShooterOwner(AArenaShooterCharacter* Character)
{
	OwningCharacter = Character;
	OwningController = Cast<AArenaShooterPlayerController>(Character->GetController());
}

void ABaseWeapon::CompletePullout()
{
	bIsActive = true;
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogWeapons, Log, TEXT("Complete pullout"));
}

void ABaseWeapon::CompleteReload()
{
	CurrAmmo = MaxAmmo;
	bIsReloading = false;
	UpdateHUD();
}

void ABaseWeapon::PlayAnim(UAnimMontage* Anim)
{
	// Get the animation object for the arms mesh
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh1P()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Anim, 1.0f);
	}
}

void ABaseWeapon::UpdateHUD()
{
	if (AArenaShooterPlayerController* Controller = Cast<AArenaShooterPlayerController>(OwningCharacter->GetController()))
	{
		if (Controller->HUD) // refactor HUD, should probably use a getter
		{
			Controller->HUD->SetAmmo(CurrAmmo, MaxAmmo);
		}
	}
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

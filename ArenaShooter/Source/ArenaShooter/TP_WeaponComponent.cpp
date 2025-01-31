// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "ArenaShooterCharacter.h"
#include "PlayerHUD.h"
#include "ArenaShooterPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Enemies/Enemy.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	ScopeFOV = 60.0f;
	ScopeMoveSpeed = 200.0f;
	MaxAmmo = 80;
	CurAmmo = MaxAmmo;
	ReloadCooldown = 2.0f;
	InverseFireRate = 0.05f;
	bIsSemiAuto = false;
	TimeSinceLastShot = 0.0f;
	bIsFiring = false;
	bIsReloading = false;
	// Enable ticking
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UTP_WeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TimeSinceLastShot += DeltaTime;
	if (bIsFiring && !bIsReloading) {
		if (Character == nullptr || Character->GetController() == nullptr || CurAmmo <= 0)
		{
			return;
		}

		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (PlayerController)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			// Start location is the camera's world location
			FVector Start = CameraLocation;
			// End location is 1000 units in the direction the camera is facing
			FVector End = Start + (CameraRotation.Vector() * 10000.0f);

			// Define collision query parameters
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(Character); // Ignore the player character
			CollisionParams.bTraceComplex = true;       // Use complex collision

			// Fire Rate check
			if (TimeSinceLastShot >= InverseFireRate) {
				FHitResult HitResult;
				// Perform the line trace
				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, CollisionParams);

				if (bHit)
				{
					// Process the hit result (e.g., apply damage or spawn effects)
					AActor* HitActor = HitResult.GetActor();
					if (HitActor)
					{
						AEnemy* Enemy = Cast<AEnemy>(HitActor);
						if (Enemy)
						{
							// Bind the weapon’s function to the enemy’s delegate
							Enemy->OnDeath.BindUObject(this, &UTP_WeaponComponent::OnEnemyKilled);
						}
						UGameplayStatics::ApplyPointDamage(HitActor, 1.0f, HitResult.ImpactPoint, HitResult, Character->GetController(), this->GetOwner(), nullptr);
					}
				}
				TimeSinceLastShot = 0.0f;

				// Decal
				UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/space-junk-11648_Mat.space-junk-11648_Mat"));
				if (DecalMaterial)
				{
					FVector DecalLocation = HitResult.ImpactPoint;
					FRotator DecalRotation = HitResult.ImpactNormal.Rotation();
					FVector DecalSize = FVector(10.0f, 10.0f, 10.0f);
					UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, DecalSize, DecalLocation, DecalRotation, 0.2f);
				}

				// Update ammo
				CurAmmo--;
				AArenaShooterPlayerController* Controller = Cast<AArenaShooterPlayerController>(PlayerController);
				if (Controller && Controller->HUD)
				{
					Controller->HUD->SetAmmo(CurAmmo, MaxAmmo);
				}

				// Try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySound2D(this, FireSound);
				}

				// Try and play a firing animation if specified
				if (FireAnimation != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
		}
		if (bIsSemiAuto) {
			bIsFiring = false;
		}
	}
}

void UTP_WeaponComponent::OnEnemyKilled()
{
	if (KillSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, KillSound);
	}
	Reload();
}

void UTP_WeaponComponent::Fire()
{
	bIsFiring = !bIsFiring;
}

void UTP_WeaponComponent::Aim()
{
	bIsAiming = !bIsAiming;

	if (bIsAiming) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
			if (CameraManager)
			{
				CameraManager->SetFOV(ScopeFOV);
			}
		}
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(Character))
		{
			UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
			if (MovementComponent)
			{
				MoveSpeed = MovementComponent->MaxWalkSpeed;
				MovementComponent->MaxWalkSpeed = ScopeMoveSpeed;
			}
		}
	}
	else {
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
			if (CameraManager)
			{
				CameraManager->SetFOV(90.0f);
			}
		}
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(Character))
		{
			UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
			if (MovementComponent)
			{
				MovementComponent->MaxWalkSpeed = MoveSpeed;
			}
		}
	}
}

void UTP_WeaponComponent::StartReload() {
	if (!bIsReloading && CurAmmo != MaxAmmo) {
		if (ReloadSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, ReloadSound);
		}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UTP_WeaponComponent::Reload, ReloadCooldown, false);
		bIsReloading = true;
	}
}

void UTP_WeaponComponent::Reload() {
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		CurAmmo = MaxAmmo;

		AArenaShooterPlayerController* Controller = Cast<AArenaShooterPlayerController>(PlayerController);
		if (Controller && Controller->HUD)
		{
			Controller->HUD->SetAmmo(CurAmmo, MaxAmmo);
		}
		bIsReloading = false;
	}
}

bool UTP_WeaponComponent::AttachWeapon(AArenaShooterCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::Fire);
			if (!bIsSemiAuto) {
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::Fire);
			}
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::Aim);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::Aim);
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::StartReload);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(WeaponMappingContext);
		}
	}
}
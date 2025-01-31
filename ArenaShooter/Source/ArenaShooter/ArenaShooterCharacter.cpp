// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaShooterCharacter.h"
#include "ArenaShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_WeaponComponent.h"
#include "ArenaShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ArenaShooterGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AArenaShooterCharacter

AArenaShooterCharacter::AArenaShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	MouseSens = 0.15f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->AirControlBoostVelocityThreshold = 0.0f;
	DashCooldown = 1.0f;
	MaxHealth = CurrHealth = 10.0f;
}

void AArenaShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CurrHealth = MaxHealth;
	bCanDash = true;
}

void AArenaShooterCharacter::EnableDash()
{
	bCanDash = true;
}

//////////////////////////////////////////////////////////////////////////// Input

void AArenaShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AArenaShooterCharacter::Dash);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArenaShooterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AArenaShooterCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

float AArenaShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Reduce health by the damage amount
	CurrHealth -= DamageAmount;

	// Check if the enemy is dead
	if (CurrHealth <= 0)
	{
		AArenaShooterGameMode* GameMode = Cast<AArenaShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->EndLevel();
			UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
		}
		// Handle death (you can call a custom death function, play an animation, etc.)
		Destroy();
	}

	// Return the amount of damage applied
	return DamageAmount;
}

void AArenaShooterCharacter::Dash(const FInputActionValue& Value)
{
	if (Controller != nullptr && bCanDash) {
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			if (DashSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, DashSound);
			}
			if (GetVelocity().IsNearlyZero()) {
				LaunchCharacter(GetActorForwardVector() * 5000.0f, false, true);
			}
			else {
				FVector DashDir = GetVelocity();
				DashDir.Z = 0.0f;
				DashDir = DashDir.GetSafeNormal();
				LaunchCharacter(DashDir * 5000.0f, false, true);
			}
			bCanDash = false;
			GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &AArenaShooterCharacter::EnableDash, DashCooldown, false);
		}
	}
}

void AArenaShooterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AArenaShooterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		LookAxisVector *= MouseSens;
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
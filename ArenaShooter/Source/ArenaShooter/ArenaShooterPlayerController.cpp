// Copyright Epic Games, Inc. All Rights Reserved.


#include "ArenaShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"

void AArenaShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (HUDClass)
	{

		HUD = CreateWidget<UPlayerHUD>(this, HUDClass);
		if (HUD)
		{
			HUD->AddToViewport();
		}
	}
}

void AArenaShooterPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HUD) {
		HUD->RemoveFromParent();
		HUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}
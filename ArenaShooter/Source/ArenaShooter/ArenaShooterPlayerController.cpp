// Copyright Epic Games, Inc. All Rights Reserved.


#include "ArenaShooterPlayerController.h"
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "ArenaShooterGameInstance.h"
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

void AArenaShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &AArenaShooterPlayerController::TogglePauseMenu);
	}
}

void AArenaShooterPlayerController::TogglePauseMenu()
{
	if (IsPauseMenuOpen == false)
	{
		// Opening pause menu
		if ((PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuType)))
		{
			PauseMenuWidget->AddToViewport();
			IsPauseMenuOpen = true;
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
			// Add this binding so it can deal with the same logic when we want to close it
			if (UButton* ResumeButton = Cast<UButton>(PauseMenuWidget->GetWidgetFromName(FName("ResumeButton"))))
			{
				ResumeButton->OnReleased.AddDynamic(this, &AArenaShooterPlayerController::TogglePauseMenu);
			}
		}
	}
	else
	{
		// Closing pause menu
		if (PauseMenuWidget)
		{
			PauseMenuWidget->RemoveFromViewport();
			PauseMenuWidget = nullptr;
			IsPauseMenuOpen = false;
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
		}
	}
}

AArenaShooterPlayerController::AArenaShooterPlayerController()
{
	IsPauseMenuOpen = false;
}

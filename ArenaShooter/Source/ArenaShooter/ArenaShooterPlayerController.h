// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "ArenaShooterPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class ARENASHOOTER_API AArenaShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	/* PAUSE MENU, but right now it is the settings menu */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuType;

	bool IsPauseMenuOpen;

	UPROPERTY()
	UUserWidget* PauseMenuWidget;


	// Begin Actor interface
protected:


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent();

	UFUNCTION()
	virtual void TogglePauseMenu();
	// End Actor interface

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPlayerHUD> HUDClass;

	class UPlayerHUD* HUD;

	AArenaShooterPlayerController();
};

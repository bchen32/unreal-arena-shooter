#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpgradeSystem.generated.h"

// Enum for upgrade types
UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
    DoubleJump UMETA(DisplayName = "Double Jump"),
    DoubleDash UMETA(DisplayName = "Double Dash"),
    Ability3 UMETA(DisplayName = "Ability 3"),
    Max UMETA(Hidden)
};

UCLASS()
class ARENASHOOTER_API UUpgradeSystem : public UObject
{
	GENERATED_BODY()

public:

	// Constructor
    UUpgradeSystem();

	// Array that holds upgrade tiers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Upgrades")
    TArray<int32> UpgradeList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Upgrades")
	ACharacter* ArenaShooterCharacter;

	// Function to unlock an upgrade and increase its tier
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
    void Upgrade(EUpgradeType UpgradeType);

	// Function to get the tier of an upgrade
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
    int32 GetUpgradeTier(EUpgradeType UpgradeType) const;

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
    void DoubleJump();

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
    void DoubleDash();
};

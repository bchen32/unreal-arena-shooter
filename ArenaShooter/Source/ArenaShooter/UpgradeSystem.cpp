#include "UpgradeSystem.h"
#include "ArenaShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Constructor
UUpgradeSystem::UUpgradeSystem()
{

    // Initialize upgrades list with the starting tier 0 (no upgrades).
    int32 NumUpgrades = static_cast<int32>(EUpgradeType::Max);
    UpgradeList.Init(0, NumUpgrades);

}
// Function to unlock an upgrade and increase its tier
void UUpgradeSystem::Upgrade(EUpgradeType UpgradeType)
{
    ArenaShooterCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    int32 UpgradeIndex = static_cast<int32>(UpgradeType);
    if (UpgradeIndex >= 0 && UpgradeIndex < UpgradeList.Num())
    {
        UpgradeList[UpgradeIndex] += 1;
    }

    if (UpgradeType == EUpgradeType::DoubleJump) {
        DoubleJump();
    }

}

// Function to get the tier of an upgrade
int32 UUpgradeSystem::GetUpgradeTier(EUpgradeType UpgradeType) const
{
    int32 UpgradeIndex = static_cast<int32>(UpgradeType);
    if (UpgradeIndex >= 0 && UpgradeIndex < UpgradeList.Num())
    {
        return UpgradeList[UpgradeIndex];
    }
    return 0;
}

void UUpgradeSystem::DoubleJump()
{
    if (ArenaShooterCharacter)
    {
        ArenaShooterCharacter->JumpMaxCount = 2;
    }
}

void UUpgradeSystem::DoubleDash()
{   
    /*
    ArenaShooterCharacter = Cast<AArenaShooterCharacter>(ArenaShooterCharacter);
    if (ArenaShooterCharacter)
    {
        ArenaShooterCharacter->maxDashes = 2;
    }
    */
}

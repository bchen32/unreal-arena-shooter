#include "UpgradeSystem.h"
#include "ArenaShooterCharacter.h"
#include "ArenaShooterGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Constructor
UUpgradeSystem::UUpgradeSystem()
{

}

void UUpgradeSystem::Initialize(AArenaShooterCharacter* Character)
{

    UArenaShooterGameInstance* GameInstance = Cast<UArenaShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GameInstance)
    {
        Owner = Character;
        UpgradeList = GameInstance->UpgradeList;

        for (int32 i = 0; i < UpgradeList.Num(); i++)
        {
            if (UpgradeList[i] > 0) // If upgrade has been unlocked
            {
                Upgrade(static_cast<EUpgradeType>(i)); // Apply upgrade
            }
        }
    }
}


// Function to unlock an upgrade and increase its tier
void UUpgradeSystem::Upgrade(EUpgradeType UpgradeType)
{
    Owner = Cast<AArenaShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


    if (UpgradeType == EUpgradeType::DoubleJump) {
        DoubleJump();
    }
    if (UpgradeType == EUpgradeType::DoubleDash) {
        DoubleDash();
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
    if (Owner)
    {
        Owner->JumpMaxCount++;
    }
}

void UUpgradeSystem::DoubleDash()
{   
    if (Owner)
    {
        Owner->maxDashes++;
    }
}

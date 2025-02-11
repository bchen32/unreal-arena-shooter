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
                Upgrade(static_cast<EUpgradeType>(i), UpgradeList[i]); // Apply upgrade
            }
        }
    }
}


void UUpgradeSystem::Upgrade(EUpgradeType UpgradeType, int32 Tier)
{
    Owner = Cast<AArenaShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


    if (UpgradeType == EUpgradeType::DoubleJump) {
        DoubleJump(Tier);
    }
    if (UpgradeType == EUpgradeType::DoubleDash) {
        DoubleDash(Tier);
    }
    if (UpgradeType == EUpgradeType::SlowMo) {
        SlowMo(Tier);
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

void UUpgradeSystem::DoubleJump(int32 Tier)
{
    if (Owner)
    {
        Owner->JumpMaxCount += Tier;
    }
}

void UUpgradeSystem::DoubleDash(int32 Tier)
{   
    if (Owner)
    {
        Owner->maxDashes += Tier;
    }
}

void UUpgradeSystem::SlowMo(int32 Tier)
{   
    if (Owner)
    {
        Owner->MaxSlowMo += 10.0f * Tier;
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetAmmo(int32 NewCurAmmo, int32 NewMaxAmmo)
{
    AmmoText->SetText(FText::FromString(FString::Printf(TEXT("Ammo: %d / %d"), NewCurAmmo, NewMaxAmmo)));
}
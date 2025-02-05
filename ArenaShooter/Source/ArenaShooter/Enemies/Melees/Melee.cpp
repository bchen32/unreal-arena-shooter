// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Melees/Melee.h"
#include "ArenaShooterCharacter.h"
#include "Engine/DamageEvents.h"

void UMelee::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;

	if (AArenaShooterCharacter* Player = Cast<AArenaShooterCharacter>(OtherActor))
	{
		Player->TakeDamage(Damage, FPointDamageEvent(), nullptr, GetOwner());
	}

}

void UMelee::BeginPlay()
{
	OnComponentBeginOverlap.AddDynamic(this, &UMelee::OnOverlap);
}

UMelee::UMelee()
{
	Damage = 5.0f;
}

void UMelee::Activate()
{
	SetGenerateOverlapEvents(true);
}

void UMelee::Disable()
{
	SetGenerateOverlapEvents(false);
}

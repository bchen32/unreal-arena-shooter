// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AerialEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

AAerialEnemy::AAerialEnemy()
{
	// Create a hitbox for the ground
	GroundComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GroundHitbox"));

	// Make the root be the child of that ground components
	USceneComponent* PreviousRootComponent = GetRootComponent();
	SetRootComponent(GroundComponent);
	PreviousRootComponent->SetupAttachment(GroundComponent);

	// Update the character movement component so it knows which component to move since it was on the previous root
	GetCharacterMovement()->SetUpdatedComponent(GroundComponent);
}

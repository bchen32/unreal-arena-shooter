// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Engine/DamageEvents.h"
#include "../../ArenaShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	HitboxComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitboxComponent"));
	HitboxComponent->InitSphereRadius(5.0f);
	HitboxComponent->BodyInstance.SetCollisionProfileName("Projectile");
	// When it hits something, expire
	HitboxComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	// Players can't walk on it
	HitboxComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	HitboxComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = HitboxComponent;


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(HitboxComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	Damage = 1.0f;
	Lifetime = 5.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// When it lives for too long, expire
	GetWorld()->GetTimerManager().SetTimer(ExpirationTimerHandle, this, &AProjectile::Expire, Lifetime, false);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (AArenaShooterCharacter* Player = Cast<AArenaShooterCharacter>(OtherActor))
	{
		// Hurt the player

		AController* Controller = nullptr;
		if (ACharacter* Character = Cast<ACharacter>(this->GetOwner())) {
			Controller = Character->GetController();
		};
		Player->TakeDamage(1.0f, FPointDamageEvent(), Controller, this->GetOwner());
	}

	// Expire the projectile if it isn't the owner
	if (OtherActor != this->GetOwner())
	{
		Expire();
	}
}

void AProjectile::Shoot(FVector Velocity)
{
	ProjectileMovementComponent->Velocity = Velocity;
}

void AProjectile::Expire()
{
	Destroy();
}

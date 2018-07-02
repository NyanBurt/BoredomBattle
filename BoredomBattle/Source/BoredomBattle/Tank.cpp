// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "Projectile.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// No need to protect ptr as it is added at construction
	//TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("AimingComponent"));
	//TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("MovementComponent"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
	
}


void ATank::AimAt(FVector HitLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at: %s"), *(GetName()), *(HitLocation.ToString()))
	if (!ensure(TankAimingComponent)) { return; }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);

}


void ATank::Fire()
{

	UE_LOG(LogTemp, Warning, TEXT("Fire!!!"))

		if (!ensure(Barrel)) { return; }

	bool isReloaded = (GetWorld()->GetTimeSeconds() - LastFireTime) > ReloadTime;

	if (isReloaded) {
		
		// Spawn a projectile
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation("FiringSocket"),
			Barrel->GetSocketRotation("FiringSocket")
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
		
	}

}


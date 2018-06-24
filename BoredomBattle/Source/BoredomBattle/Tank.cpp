// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// No need to protect ptr as it is added at construction
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("AimingComponent"));

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::AimAt(FVector HitLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at: %s"), *(GetName()), *(HitLocation.ToString()))
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);

}

void ATank::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	if (!BarrelToSet) {
		UE_LOG(LogTemp, Error, TEXT("Barrel hasn't been assigned"))
		return;
	}
	TankAimingComponent->SetBarrelReference(BarrelToSet);
}

void ATank::SetTurretReference(UTankTurret * TurretToSet)
{
	if (!TurretToSet) {
		UE_LOG(LogTemp, Error, TEXT("Turret hasn't been assigned"))
		return;
	}
	TankAimingComponent->SetTurretReference(TurretToSet);
}

void ATank::Fire()
{

	UE_LOG(LogTemp, Warning, TEXT("Fire!!!"))

}


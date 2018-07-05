// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::Initialise(UTankTurret * TurretToSet, UTankBarrel * BarrelToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// So that first fire is after initial reload
	LastFireTime = GetWorld()->GetTimeSeconds();
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTime) {
		AimingStatus = EAimingStatus::Reloading;
	}
	else if (IsBarrelMoving()) {
		//UE_LOG(LogTemp, Warning, TEXT("Aiming"))
		AimingStatus = EAimingStatus::Aiming;
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Locked"))
		AimingStatus = EAimingStatus::Locked;
	}

}

void UTankAimingComponent::AimAt(FVector AimLocation)
{

	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *(GetOwner()->GetName()),	*(AimLocation.ToString()), *(Barrel->GetComponentLocation().ToString()))
	if (!ensure(Barrel)) {
		UE_LOG(LogTemp, Error, TEXT("Barrel hasn't been assigned"))
		return;
	}
	FVector OUTTossVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("FiringSocket"));
	
	if (UGameplayStatics::SuggestProjectileVelocity(
			this,
			OUTTossVelocity,
			StartLocation,
			AimLocation,
			LaunchSpeed,
			false,
			0.0f,
			0.0f,
			ESuggestProjVelocityTraceOption::DoNotTrace

		)) {
		AimDirection = OUTTossVelocity.GetSafeNormal();
		//UE_LOG(LogTemp, Warning, TEXT("Aiming direction: %s"), *(AimDirection.ToString()))

		MoveBarrel(AimDirection);
	}

	

}

void UTankAimingComponent::MoveBarrel(FVector AimDirection)
{

	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	// Work out differences between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAtRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAtRotator - BarrelRotator;
	
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) > 180.0) {
		DeltaRotator.Yaw *= -1;
	}
	Turret->Rotate(DeltaRotator.Yaw);

}

bool UTankAimingComponent::IsBarrelMoving()
{

	if (!ensure(Barrel)) { return false; }
	FVector BarrelFwd = Barrel->GetForwardVector();
	return !BarrelFwd.Equals(AimDirection, 0.01f);

}

void UTankAimingComponent::Fire()
{

	if (AimingStatus != EAimingStatus::Reloading) {
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
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


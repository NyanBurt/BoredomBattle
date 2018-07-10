// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"

#define OUT

void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();
	
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) {	return;	}
	FoundAimingComponent(AimingComponent);

}

void ATankPlayerController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
	
}

/*
void ATankPlayerController::FoundAimingComponent(UTankAimingComponent* AimingCompRef)
{
	// ...
}
*/

void ATankPlayerController::AimTowardsCrosshair() {
	
	if (!GetPawn()) { return; }	// If not possessing
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // OUT parameter

	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	//UE_LOG(LogTemp, Warning, TEXT("bGotHitLocation: %i"), bGotHitLocation)
	if (bGotHitLocation) {
		
		// TODO Tell controlled tank to aim at location
		AimingComponent->AimAt(HitLocation);

	}

}

// Get world location through crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& OUTHitLocation) const {

	// Find the crosshair location
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto CrosshairRelativeLocation = FVector2D(
		(CrosshairXLocation * (float)ViewportSizeX),
		(CrosshairYLocation * (float)ViewportSizeY)
	);
	
	// De-project the screen location of the crosshair to a world location
	FVector CrosshairWorldLocation, CrosshairWorldDirection;

	if (DeprojectScreenPositionToWorld(
		CrosshairRelativeLocation.X,
		CrosshairRelativeLocation.Y,
		CrosshairWorldLocation,
		CrosshairWorldDirection
		)) {
		//UE_LOG(LogTemp, Warning, TEXT("Direction: %s"), *(CrosshairWorldDirection.ToString()))
		// Linetrace along Look direction up to a maximum range
		FHitResult OUTHitResult;
		
		if (GetWorld()->LineTraceSingleByChannel(
			OUTHitResult,
			CrosshairWorldLocation,
			CrosshairWorldLocation + CrosshairWorldDirection * LineTraceRange,
			ECollisionChannel::ECC_Camera
		)
			) {
			OUTHitLocation = OUTHitResult.Location;
			UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
			AimingComponent->AimAt(OUTHitLocation);
			//auto Time = GetWorld()->GetTimeSeconds();
			//UE_LOG(LogTemp, Warning, TEXT("GetSightRayHitLocation success %f"), Time)
			//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(OUTHitResult.Actor->GetName()))
			return true;
		}

	}
	
	OUTHitLocation = FVector(0.0f);
	//auto Time = GetWorld()->GetTimeSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("GetSightRayHitLocation failed %f"), Time)
	return false;	// WARNING: TEMPORARY

}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Player tank receiving!"))
	if (!GetPawn()) { return; }
	StartSpectatingOnly();
}

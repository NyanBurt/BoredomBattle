// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"

#define OUT

void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();
	
	UTankAimingComponent* AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent)) {
		FoundAimingComponent(AimingComponent);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't find AimingComponent"))
	}
	

}

void ATankPlayerController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
	
}

ATank* ATankPlayerController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());

}

/*
void ATankPlayerController::FoundAimingComponent(UTankAimingComponent* AimingCompRef)
{
	// ...
}
*/

void ATankPlayerController::AimTowardsCrosshair() {
	
	if (!ensure(GetControlledTank())) {
		return;
	}

	FVector HitLocation; // OUT parameter

	if (GetSightRayHitLocation(HitLocation)) {
		
		// TODO Tell controlled tank to aim at location


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
			ECollisionChannel::ECC_Visibility

		)) {
			OUTHitLocation = OUTHitResult.Location;
			GetControlledTank()->AimAt(OUTHitLocation);
			//auto Time = GetWorld()->GetTimeSeconds();
			//UE_LOG(LogTemp, Warning, TEXT("GetSightRayHitLocation success %f"), Time)
			return true;
		}

	}
	
	OUTHitLocation = FVector(0.0f);
	//auto Time = GetWorld()->GetTimeSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("GetSightRayHitLocation failed %f"), Time)
	return false;	// WARNING: TEMPORARY

}

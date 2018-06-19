// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();
	ATank* tmpTankPtr = GetControlledTank();
	if (tmpTankPtr) {
		UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn: %s"), *(tmpTankPtr->GetName()))
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Controlled Pawn is null"))
	}

}

ATank* ATankPlayerController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());

}



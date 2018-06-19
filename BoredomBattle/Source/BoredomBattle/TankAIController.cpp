// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

void ATankAIController::BeginPlay() {

	Super::BeginPlay();
	ATank* tmpTankPtr = GetControlledTank();
	if (tmpTankPtr) {
		UE_LOG(LogTemp, Warning, TEXT("AI Controlled Pawn: %s"), *(tmpTankPtr->GetName()))
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("AI Controlled Pawn is null"))
	}

}

ATank* ATankAIController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());

}



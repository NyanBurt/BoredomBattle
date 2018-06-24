// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {

	Super::BeginPlay();
	ATank* tmpTankPtr = GetPlayerTank();
	if (tmpTankPtr) {
		UE_LOG(LogTemp, Warning, TEXT("Player Controlled Pawn: %s"), *(tmpTankPtr->GetName()))
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("AI Controlled Pawn is null"))
	}

}

void ATankAIController::Tick(float DeltaTime)
{
	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank) {
		GetControlledTank()->AimAt(PlayerTank->GetActorLocation());
	}
	
}

ATank* ATankAIController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());

}

ATank* ATankAIController::GetPlayerTank() const {

	ATank* PlayerTankPtr = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerTankPtr) {
		return PlayerTankPtr;
	}
	else {
		return nullptr;
	}
	

}



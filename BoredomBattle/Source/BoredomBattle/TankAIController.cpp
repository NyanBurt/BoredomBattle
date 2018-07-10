// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {

	Super::BeginPlay();


}

void ATankAIController::Tick(float DeltaTime)
{
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	if (!ensure(PlayerTank && ControlledTank)) {
		return;
	}

	MoveToActor(PlayerTank, AcceptanceRadius);
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	/*
	if (AimingComponent->GetFiringState() == EAimingStatus::Reloading)
		UE_LOG(LogTemp, Warning, TEXT("TankAI aiming status: reloading"))

	if (AimingComponent->GetFiringState() == EAimingStatus::Aiming)
		UE_LOG(LogTemp, Warning, TEXT("TankAI aiming status: aiming"))

	if (AimingComponent->GetFiringState() == EAimingStatus::Locked)
		UE_LOG(LogTemp, Warning, TEXT("TankAI aiming status: locked"))
	*/

	if (AimingComponent->GetFiringState() == EAimingStatus::Locked)
		AimingComponent->Fire();
	
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{

	UE_LOG(LogTemp, Warning, TEXT("AI tank receiving!"))
	if (!GetPawn()) { return;	}
	GetPawn()->DetachFromControllerPendingDestroy();

}



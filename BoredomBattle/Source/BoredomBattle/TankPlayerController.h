// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BOREDOMBATTLE_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:
	
	void AimTowardsCrosshair();
	bool GetSightRayHitLocation(FVector& OUTHitLocation) const;

protected:
	// DO NOT DEFINE BlueprintImplementableEvent
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);
	
};

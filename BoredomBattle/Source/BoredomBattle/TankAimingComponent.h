// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming states
UENUM()
enum class EAimingStatus : uint8 {
	Locked,
	Aiming,
	Reloading
};

// FORWARD DECLARATION EXAMPLE!!!
class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOREDOMBATTLE_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTurret* TurretToSet, UTankBarrel* BarrelToSet);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly, Category = "State")
		EAimingStatus AimingStatus = EAimingStatus::Aiming;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AimAt(FVector AimLocation, float LaunchSpeed);

private:
	UTankBarrel * Barrel = nullptr;
	UTankTurret * Turret = nullptr;

	void MoveBarrel(FVector AimDirection);

};

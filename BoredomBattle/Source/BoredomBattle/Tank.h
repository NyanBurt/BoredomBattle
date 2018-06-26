// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankBarrel;
class UTankTurret;
class UTankAimingComponent;
class AProjectile;


UCLASS()
class BOREDOMBATTLE_API ATank : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent * TankAimingComponent = nullptr;

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 8000;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//UClass* ProjectileBlueprint;
	TSubclassOf<AProjectile> ProjectileBlueprint;
	
	// Local barrel reference for spawning projectile
	UTankBarrel* Barrel = nullptr;	// TODO: remove

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTime = 3.0f;

	float LastFireTime = 0;

};

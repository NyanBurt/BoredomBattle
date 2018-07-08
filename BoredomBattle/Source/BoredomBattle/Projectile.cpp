// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->bAutoActivate = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("CollisionMesh"));
	SetRootComponent(CollisionMesh);
	//GetOwner()->SetRootComponent(Cast<USceneComponent>(CollisionMesh));	
	//Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent())->SetNotifyRigidBodyCollision(true);	//CRASHES
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(true);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("LaunchBlast"));
	LaunchBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("ImpactBlast"));
	ImpactBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("ExplosionForce"));
	ExplosionForce->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovement->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();

	ExplosionForce->FireImpulse();

	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>()	// Damage all actors
	);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::OnTimerExpire, DestroyDelay, false);
}

void AProjectile::OnTimerExpire() {
	Destroy();
}


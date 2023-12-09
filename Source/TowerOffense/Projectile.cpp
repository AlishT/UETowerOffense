// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include <Net/UnrealNetwork.h>

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColllisionSphere"));
	SetRootComponent(ProjectileCollisionSphere);

	PojectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PojectileMesh"));
	PojectileMesh->SetupAttachment(ProjectileCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileCollisionSphere);
	ProjectileMovementComponent->InitialSpeed = 5000.f;
	ProjectileMovementComponent->MaxSpeed = 5000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 5.f;

	ProjectileCollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
} 

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::FireDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!Hit.GetActor())
	{
		return;
	}
	
	AController* OwnerInstigator = GetOwner()->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (!OtherActor) 
	{
		return;
	}

	UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerInstigator, this, DamageTypeClass);

	if (!IsNetMode(NM_DedicatedServer))
	{
		if (HitPartical)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPartical, GetActorLocation());
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
	}

	Destroy();

	UE_LOG(LogTemp, Warning, TEXT("Destroy"));
	
}

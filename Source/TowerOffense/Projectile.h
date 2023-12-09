// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class TOWEROFFENSE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USphereComponent* ProjectileCollisionSphere = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UStaticMeshComponent* PojectileMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* HitPartical = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 Damage = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* HitSound = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireDirection(const FVector& ShootDirection);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

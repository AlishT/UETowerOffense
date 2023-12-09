// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "TowerPawn.generated.h"

class USphereComponent;
class UWidgetComponent;
class UPawnSensingComponent;
/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()

public:
	ATowerPawn();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USphereComponent* CollisionSphere = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetTypeClass")
	TSubclassOf<AActor> TargetClass;

	UPROPERTY(Transient)
	AActor* Target = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float ShotsPerSec = 0.f;

	float TimeAfterShot = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (Units = "Degrees"))
	float TurretAimError = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UWidgetComponent* HealthBarComponent = nullptr;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

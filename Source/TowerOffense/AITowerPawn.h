// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerPawn.h"
#include "AITowerPawn.generated.h"

class UWidgetComponent;
class UPawnSensingComponent;
class UBlackboardComponent;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API AAITowerPawn : public ATurretPawn
{
	GENERATED_BODY()

	AAITowerPawn();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USphereComponent* CollisionSphere = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "TargetTypeClass")
	TSubclassOf<AActor> TargetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float ShotsPerSec = 0.f;

	float TimeAfterShot = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (Units = "Degrees"))
	float TurretAimError = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UWidgetComponent* HealthBarComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UPawnSensingComponent* PawnSensingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FName KeyName = "None";

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	AActor* Target = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UBehaviorTree* BehaviorTree = nullptr;

	UFUNCTION()
	void OnSeeEnemy(APawn* Pawn);

	UFUNCTION(NetMulticast, reliable)
	void RPCTurretShoot();

	UFUNCTION(NetMulticast, reliable)
	void RPCRotateTurret(FVector Location);

};

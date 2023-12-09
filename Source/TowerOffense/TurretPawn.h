// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponentOwner.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

class UCapsuleComponent;
class UParticleSystem;
class AProjectile;
class UHealthComponent;
class UCameraShake;
class UAudioComponent;
class UHealthBarWidget;

UCLASS()
class TOWEROFFENSE_API ATurretPawn : public APawn, public IHealthComponentOwner
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UStaticMeshComponent* BaseMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UStaticMeshComponent* TurretMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName TeamColorMaterialParamName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (GetOptions = "GetMaterialNames"))
	FName MaterialSlotName = NAME_None;

	FVector TargetLocation = FVector::ZeroVector;
	
	//UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ForceUnits = "\u00B0"))
	float InterpRotateSpeed = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (Units = "Degrees"))
	float TurretScatter = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* MuzzleFlash = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* ExplosivePartical = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* ExplosiveSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* ShotSound = nullptr;

	//float Kills = 0.f;
	//UPROPERTY(BlueprintReadOnly)
		//float CurrentHealth = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FColor TeamColor = FColor::White;

	UFUNCTION(BlueprintCallable)
	void SetTeamColor(const FName& ParamName, const FColor& Color, UStaticMeshComponent* Mesh);
	
	UFUNCTION()
	TArray<FString> GetMaterialNames() const;

	UFUNCTION(BlueprintCallable)
	void RotateTurret(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable)
	void TurretShoot();
	
	UFUNCTION()
	void HealthChanged(float NewHealth, float Delta);

	virtual UHealthComponent* GetHealthComponentOwner_Implementation() override;
};

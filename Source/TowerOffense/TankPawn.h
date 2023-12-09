// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "InputActionValue.h"
#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ABasePlayerController;

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ATankPawn : public ATurretPawn
{
	GENERATED_BODY()

public:

	ATankPawn();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContex = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveForward = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* TurnRight = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* InputFire = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float MaxSpeed = 0.f;

	UPROPERTY()
	ABasePlayerController* PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USceneComponent* LeftTrackDustSpawnPoint = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USceneComponent* RightTrackDustSpawnPoint = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* TrackDust = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	UAudioComponent* EngineSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	bool bIsMove = false;
	
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float Acceleration = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 MaxTankAmmo = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerLocation)
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerRotation)
	FRotator PlayerRotation = FRotator::ZeroRotator;

	UPROPERTY(ReplicatedUsing = OnRep_TurretRotate)
	FVector TargetPosition = FVector::ZeroVector;

	float NewSpeed = 0.f;

	float ServerErrorPercent = 0.1f;

public:

	UPROPERTY(BlueprintReadOnly, Replicated)//Using = OnRep_UpdateAmmo)
	int32 CurrentTankAmmo = 0.f;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Move(const FInputActionValue& Value);

	void Turn(const FInputActionValue& Value);

	void Fire();

	UFUNCTION(Server, reliable)
	void Server_Fire(float Ammo);

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPCFire();

	UFUNCTION(Server, reliable)
	void Server_TurretRotate(FVector Location);

	UFUNCTION()
	void OnRep_TurretRotate();

	UFUNCTION()
	void UpdateTeamColor();

	UFUNCTION(Server, reliable)
	void ServerUpdateMove(float Value, float ClientsSpeed);

	UFUNCTION(Server, reliable)
	void ServerUpdateRotation(float Value);
	
	UFUNCTION()
	void OnRep_PlayerLocation();
	
	UFUNCTION()
	void OnRep_PlayerRotation();
	
	UFUNCTION()
	void OnRep_UpdateAmmo(float Ammo);
};


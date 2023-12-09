// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponentOwner.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class ATurretPawn;
/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float TurretHealth = 0.f;

	float MaxTurretHealth = 0.f;

	UFUNCTION()
	void SetTurretsHelth(float CurrentHealth, float Delta);
	
public:
	UFUNCTION()
	void SetupTurretHealthBar(APawn* TurretPawn);
	
};

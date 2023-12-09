// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> HeathBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> EnemyCountWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> AmmoCountWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Hide timer Widget")
	float TimeShowWidget = 0.f;
	
	UPROPERTY(Transient)
	UUserWidget* HealthBar = nullptr;

	UPROPERTY(Transient)
	UUserWidget* AmmoCountWidget = nullptr;

public:

	bool bIsWidgetsHidden = false;

	float CurrentTime = 0.f;;

	virtual void Tick(float DeltaTime) override;

	void SetWidgetVibility(bool bIsHidden);

	void InitializeHUDState();
	
};

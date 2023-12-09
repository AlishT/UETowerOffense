// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UStartTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, Category = "Default")
	float StartTime = 0.f;

	UFUNCTION()
	void SetTime(float Time);
};

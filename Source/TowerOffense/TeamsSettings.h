// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TeamsSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "TeamsSettings"))
class TOWEROFFENSE_API UTeamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere)
	TArray<FColor> Teams;
};

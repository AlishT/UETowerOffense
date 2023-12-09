// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TeamPlayersStart.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ATeamPlayersStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 TeamStartID = 0;
	
	UPROPERTY(EditAnywhere)
	bool bEmpty = true;
};

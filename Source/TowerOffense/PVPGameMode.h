// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PVPGameMode.generated.h"

class ABasePlayerController;
/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API APVPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	int32 CurrentTeamID = 0;

	int32 PlayerCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 MaxPlayers = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 MaxTeamPlayers = 0;

	int32 MaxTeams = 0;

	UPROPERTY(Transient)
	ABasePlayerController* CurrentPlayer = nullptr;

	UFUNCTION()
	void HandleKilledTeam(APawn* DeadPlayer);

	virtual void PostLogin(APlayerController* NewPlayer) override;

	FVector TakePlayerStart(int32 TeamID);
};

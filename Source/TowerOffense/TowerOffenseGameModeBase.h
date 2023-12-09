// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerOffenseGameModeBase.generated.h"

class ABasePlayerController;

/**
 * 
 */

UCLASS()
class TOWEROFFENSE_API ATowerOffenseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SetEnemyPawns)
	//int32 EnemyPawns = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnemyPawn")
	TSubclassOf<AActor> EnemyPanwClass;

	UPROPERTY(Transient)
	ABasePlayerController* PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	TArray<APlayerController*> PlayerControllerList;

public:
	UPROPERTY(BlueprintReadOnly)
	int32 EnemyPawns = 0;

	int32 CurrentPlayerCount = 0;

	UFUNCTION()
	void HandlePawnKilled(ATurretPawn* DeadPawn);

	virtual void PostLogin(APlayerController* NewPlayer) override;

};

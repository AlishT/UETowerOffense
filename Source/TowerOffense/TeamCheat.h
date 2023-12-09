// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "TeamCheat.generated.h"


#ifndef USING_CHEAT_MANAGER
#define USING_CHEAT_MANAGER (1 && !UE_BUILD_SHIPPING)
#endif
/**
 * 
 */
UCLASS(config = Game, Within = PlayerController)
class TOWEROFFENSE_API UTeamCheat : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void SetNewTeam(int32 InTeamID);

};

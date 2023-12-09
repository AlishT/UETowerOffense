// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotateTurret.generated.h"

//Is`s second variant
/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UBTTask_RotateTurret : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
		
	UBTTask_RotateTurret(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

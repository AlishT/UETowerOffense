// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Patrolling.generated.h"

class AAITowerPawn;
/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UBTTask_Patrolling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_Patrolling(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FRotator RotatePawn = FRotator::ZeroRotator;

	float RotateSpeed = 0.7f;

};

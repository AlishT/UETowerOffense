// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateTurret.h"
#include "AITowerPawn.h"
#include "AITowerController.h"


//Is`s second variant
UBTTask_RotateTurret::UBTTask_RotateTurret(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

EBTNodeResult::Type UBTTask_RotateTurret::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}

	AAITowerController* AIController = Cast<AAITowerController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		AAITowerPawn* AITowerPawn = Cast<AAITowerPawn>(AIController->GetPawn());

		if (AITowerPawn)
		{
			FVector TargetLocation = AITowerPawn->Target->GetActorLocation();
			AITowerPawn->RPCRotateTurret(TargetLocation);
		}
	}

	return EBTNodeResult::Succeeded;
}

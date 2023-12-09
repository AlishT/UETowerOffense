// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrolling.h"
#include "AITowerPawn.h"
#include "AITowerController.h"

UBTTask_Patrolling::UBTTask_Patrolling(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			RotatePawn = AITowerPawn->GetActorRotation();
			RotatePawn.Yaw += RotateSpeed;

			AITowerPawn->SetActorRotation(RotatePawn);
		}
	}
	return EBTNodeResult::Succeeded;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AITowerPawn.h"
#include "AITowerController.h"

UBTTask_Shoot::UBTTask_Shoot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			AITowerPawn->RPCTurretShoot();
		}
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_Shoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("TiCK Shoot"));

	if (!OwnerComp.GetAIOwner()) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}



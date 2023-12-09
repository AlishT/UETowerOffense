// Fill out your copyright notice in the Description page of Project Settings.


#include "AITowerController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AITowerPawn.h"


AAITowerController::AAITowerController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehavierTree"));

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AAITowerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAITowerPawn* AITowerPawn = Cast<AAITowerPawn>(InPawn);

	if (AITowerPawn && AITowerPawn->BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*AITowerPawn->BehaviorTree->BlackboardAsset);

		BehaviorTreeComponent->StartTree(*AITowerPawn->BehaviorTree);
	}
}

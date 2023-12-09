// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITowerController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API AAITowerController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAITowerController();

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UBlackboardComponent* BlackboardComponent = nullptr;

	//UPROPERTY(EditDefaultOnly, Category = "Default")


};

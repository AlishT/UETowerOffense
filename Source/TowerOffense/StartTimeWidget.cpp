// Fill out your copyright notice in the Description page of Project Settings.


#include "StartTimeWidget.h"
#include "TowerOffenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerController.h"

void UStartTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AGameStateBase* GameState2 = GetWorld()->GetGameState();
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerController->OnStartGameTime.AddDynamic(this, &UStartTimeWidget::SetTime);
}

void UStartTimeWidget::SetTime(float Time)
{
	StartTime = Time;
}

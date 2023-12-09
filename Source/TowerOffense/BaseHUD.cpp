// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HeathBarWidgetClass)
	{
		HealthBar = CreateWidget(GetGameInstance(), HeathBarWidgetClass);
		HealthBar->AddToViewport();
		//HealthBar->SetVisibility(ESlateVisibility::Visible);
	}

	if (EnemyCountWidgetClass)
	{
		UUserWidget* EnemyCountWidget = CreateWidget(GetGameInstance(), EnemyCountWidgetClass);
		EnemyCountWidget->AddToViewport();
	}

	if (AmmoCountWidgetClass)
	{
		AmmoCountWidget = CreateWidget(GetGameInstance(), AmmoCountWidgetClass);
		AmmoCountWidget->AddToViewport();
		//AmmoCountWidget->SetVisibility(ESlateVisibility::Visible);
	}

	bIsWidgetsHidden = true;
}

void ABaseHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;
	if (CurrentTime >= TimeShowWidget)
	{
		bIsWidgetsHidden = false;
	}
	
	SetWidgetVibility(bIsWidgetsHidden);
}

void ABaseHUD::SetWidgetVibility(bool bIsHidden)
{

	ESlateVisibility Type = (bIsHidden) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	AmmoCountWidget->SetVisibility(Type);
	HealthBar->SetVisibility(Type);
}

void ABaseHUD::InitializeHUDState()
{
	bIsWidgetsHidden = true;
	CurrentTime = 0.f;
}

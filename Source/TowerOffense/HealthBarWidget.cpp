// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "HealthComponent.h"
#include "BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponentOwner.h"
#include "BaseHUD.h"

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UHealthBarWidget::SetTurretsHelth(float CurrentHealth, float Delta)
{
	TurretHealth = CurrentHealth / MaxTurretHealth;

	ABaseHUD* BaseHUD = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (BaseHUD)
	{
		BaseHUD->InitializeHUDState();
	}
}

void UHealthBarWidget::SetupTurretHealthBar(APawn* TurretPawn)
{
	IHealthComponentOwner* HealthComponentOwnerInterface = Cast<IHealthComponentOwner>(TurretPawn);

	UHealthComponent* HealthComponent = HealthComponentOwnerInterface->Execute_GetHealthComponentOwner(TurretPawn);

	if (HealthComponent)
	{
		MaxTurretHealth = HealthComponent->GetMaxHealth();
		TurretHealth = MaxTurretHealth;

		HealthComponent->OnHealthChanged.AddDynamic(this, &UHealthBarWidget::SetTurretsHelth);
	}
	
}

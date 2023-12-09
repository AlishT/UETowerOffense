// Fill out your copyright notice in the Description page of Project Settings.


#include "AITowerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "TowerHealthBarWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AITowerController.h"

AAITowerPawn::AAITowerPawn() 
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarComponent = CreateAbstractDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAITowerPawn::OnSeeEnemy);
	
}

void AAITowerPawn::BeginPlay()
{
	Super::BeginPlay();

	UTowerHealthBarWidget* TowerHealthBarWidget = Cast<UTowerHealthBarWidget>(HealthBarComponent->GetWidget());

	TowerHealthBarWidget->SetupTurretHealthBar(this);
}

void AAITowerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Rotate Tower HealthBarWidget to Player camera;
	if (!IsNetMode(NM_DedicatedServer))
	{
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

		FVector HealhtBarLoacation = HealthBarComponent->GetComponentLocation();
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		FVector PlayerCameraLocation = CameraLocation - HealhtBarLoacation;
		FRotator HealthBarRotation = PlayerCameraLocation.Rotation();
		HealthBarComponent->SetWorldRotation(HealthBarRotation);
	}
}

void AAITowerPawn::OnSeeEnemy(APawn* Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("TEST!!!"));

	AAITowerController* AIController = Cast<AAITowerController>(Controller);
	
	if (AIController)
	{
		AIController->BlackboardComponent->SetValueAsBool(KeyName, true);
	}

	Target = Cast<AActor>(Pawn);
}

void AAITowerPawn::RPCTurretShoot_Implementation()
{
	TurretShoot();
}

void AAITowerPawn::RPCRotateTurret_Implementation(FVector Location)
{
	RotateTurret(Location);
}
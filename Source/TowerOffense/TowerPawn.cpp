// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "TowerHealthBarWidget.h"

ATowerPawn::ATowerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Clollision"));
	CollisionSphere->SetupAttachment(CapsuleComponent);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATowerPawn::OnBeginOverlap);
	
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ATowerPawn::OnEndOverlap);

	HealthBarComponent = CreateAbstractDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void ATowerPawn::BeginPlay()
{
	Super::BeginPlay();

	UTowerHealthBarWidget* TowerHealthBarWidget = Cast<UTowerHealthBarWidget>(HealthBarComponent->GetWidget());

	TowerHealthBarWidget->SetupTurretHealthBar(this);
}


void ATowerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsNetMode(NM_DedicatedServer))
	{
		//Rotate Tower HealthBarWidget to Player camera;
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

		FVector HealhtBarLoacation = HealthBarComponent->GetComponentLocation();
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		FVector PlayerCameraLocation = CameraLocation - HealhtBarLoacation;
		FRotator HealthBarRotation = PlayerCameraLocation.Rotation();
		HealthBarComponent->SetWorldRotation(HealthBarRotation);
	}
	if (!IsValid(Target))
	{
		return;
	}

	TargetLocation = Target->GetActorLocation();
	
	RotateTurret(TargetLocation);

	FVector CurrentDirection = TurretMesh->GetForwardVector();
	FVector TargetDirection = TargetLocation - TurretMesh->GetComponentLocation();

	float RotationAngleCosine = CurrentDirection.CosineAngle2D(TargetDirection);
	float RotationAngle = FMath::RadiansToDegrees(FMath::Acos(RotationAngleCosine));

	if (RotationAngle <= TurretAimError)
	{
		TimeAfterShot += DeltaTime;
		const float TimeBetweenShots = 1 / ShotsPerSec;
		
		if (TimeAfterShot >= TimeBetweenShots)
		{
			TurretShoot();
			TimeAfterShot = 0;
		}
	}
}

void ATowerPawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

	if (!TargetClass)
	{
		return;
	}
	
	if (!OtherActor->IsA(TargetClass))
	{
		return;
	}

	Target = Cast<AActor>(OtherActor);

}

void ATowerPawn::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Target == OtherActor)
	{
		Target = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
}


//float DirectionDotProduct = FVector::DotProduct(CurrentDirection, TargetDirection);
//float DirectionAngle = FMath::Acos(DirectionDotProduct) * (180.0f / PI);
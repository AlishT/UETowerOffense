// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "BaseHUD.h"
#include "BasePlayerController.h"
#include <Net/UnrealNetwork.h>
#include "Misc/App.h"

ATankPawn::ATankPawn() 
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(CapsuleComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	LeftTrackDustSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDust"));
	LeftTrackDustSpawnPoint->SetupAttachment(BaseMesh);

	RightTrackDustSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightDust"));
	RightTrackDustSpawnPoint->SetupAttachment(BaseMesh);

	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSound->SetupAttachment(CapsuleComponent);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ABasePlayerController>(Controller);
	
	if (!PlayerController)
	{
		return;
	}

	PlayerController->OnStartGame();

	PlayerController->bShowMouseCursor = true;

	CurrentTankAmmo = MaxTankAmmo;
}



void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMove)
	{
		Speed += Acceleration * DeltaTime;
		Speed = FMath::Clamp(Speed, 0, MaxSpeed);
	}
	
	if (!bIsMove)
	{
		Speed = 0.f;
		
		NewSpeed = 0.f;
	}

	if (EngineSound)
	{
		EngineSound->SetFloatParameter("RMS", Speed);
	}
	bIsMove = false;

	FHitResult HitRes = FHitResult();

	if (!PlayerController)
	{
		return;
	}

	bool bHit = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitRes);

	if (!bHit)
	{
		return;
	}

	if (!HitRes.bBlockingHit)
	{
		return;
	}

	TargetLocation = HitRes.ImpactPoint;

	Server_TurretRotate(TargetLocation);

}

void ATankPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (IsLocallyControlled())
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

		if (!EnhancedInputComponent)
		{
			return;
		}

		EnhancedInputComponent->BindAction(MoveForward, ETriggerEvent::Triggered, this, &ATankPawn::Move);
		EnhancedInputComponent->BindAction(TurnRight, ETriggerEvent::Triggered, this, &ATankPawn::Turn);
		EnhancedInputComponent->BindAction(InputFire, ETriggerEvent::Triggered, this, &ATankPawn::Fire);
	}
}

void ATankPawn::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	if (!Controller && DirectionValue == 0)
	{
		return;
	}
	
	bIsMove = true;

	FVector Location = FVector(DirectionValue, 0.f, 0.f) * Speed;
	
	AddActorLocalOffset(Location, true, 0, ETeleportType::TeleportPhysics);

	//FVector Location = GetActorForwardVector().GetSafeNormal() * Speed * DirectionValue;
	//CapsuleComponent->SetPhysicsLinearVelocity(Location);

	UE_LOG(LogTemp, Warning, TEXT("Move"));

	FVector LeftTrackDustLocation = LeftTrackDustSpawnPoint->GetComponentLocation();
	FVector RightTrackDustLocation = RightTrackDustSpawnPoint->GetComponentLocation();

	if (TrackDust)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrackDust, LeftTrackDustLocation);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrackDust, RightTrackDustLocation);
	}
	
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerUpdateMove(DirectionValue, Speed);
	}
}

void ATankPawn::Turn(const FInputActionValue& Value)
{
	const float RotationValue = Value.Get<float>();

	if (!Controller && RotationValue == 0)
	{
		return;
	}

	FRotator Rotation = FRotator(0.f, RotationValue, 0.f);

	AddActorLocalRotation(Rotation, true, 0, ETeleportType::TeleportPhysics);

	UE_LOG(LogTemp, Warning, TEXT("Turn"));
	
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerUpdateRotation(RotationValue);
	}
}

void ATankPawn::Fire()
{ 
	CurrentTankAmmo--;

	if(CurrentTankAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloud!!"));
		CurrentTankAmmo = MaxTankAmmo;

		return;
	}
	
	TurretShoot();

	if (IsLocallyControlled())
	{
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(CameraShakeClass);

			ABaseHUD* BaseHUD = Cast<ABaseHUD>(PlayerController->GetHUD());

			if (BaseHUD)
			{
				BaseHUD->InitializeHUDState();
			}
		}
	
		UE_LOG(LogTemp, Warning, TEXT("Fire"));

		Server_Fire(CurrentTankAmmo);
	}
}

void ATankPawn::Server_Fire_Implementation(float Ammo)
{
#if WITH_SERVER_CODE
	CurrentTankAmmo--;

	if (CurrentTankAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloud!!"));
		CurrentTankAmmo = MaxTankAmmo;

		return;
	}
	    
	if (CurrentTankAmmo == Ammo)
	{
		OnRep_UpdateAmmo(CurrentTankAmmo);

		TurretShoot();
		
		MulticastRPCFire();

		UE_LOG(LogTemp, Warning, TEXT("Fire on server"));
	}
#endif
}

void ATankPawn::MulticastRPCFire_Implementation()
{
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		TurretShoot();
	}
}

void ATankPawn::OnRep_UpdateAmmo(float Ammo)
{
	CurrentTankAmmo = Ammo;
}

void ATankPawn::Server_TurretRotate_Implementation(FVector Location)
{
	TargetPosition = Location;
	OnRep_TurretRotate();
}

void ATankPawn::OnRep_TurretRotate()
{
	RotateTurret(TargetPosition);
}

void ATankPawn::UpdateTeamColor()
{
	SetTeamColor(TeamColorMaterialParamName, TeamColor, BaseMesh);
	SetTeamColor(TeamColorMaterialParamName, TeamColor, TurretMesh);
}

void ATankPawn::OnRep_PlayerLocation()
{
	SetActorLocation(PlayerLocation);
}

void ATankPawn::OnRep_PlayerRotation()
{
	SetActorRotation(PlayerRotation);
}

void ATankPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATankPawn, PlayerLocation);
	DOREPLIFETIME(ATankPawn, PlayerRotation);
	DOREPLIFETIME(ATankPawn, TargetPosition);
	DOREPLIFETIME(ATankPawn, CurrentTankAmmo);
}

void ATankPawn::ServerUpdateMove_Implementation(float Value, float ClientsSpeed)
{
	if (Value != 0 && Value <= 1)
	{
		NewSpeed += Acceleration * GetWorld()->GetDeltaSeconds();
		NewSpeed = FMath::Clamp(NewSpeed, 0, MaxSpeed);

		UE_LOG(LogTemp, Warning, TEXT("Speed on server %f"), ClientsSpeed);
		UE_LOG(LogTemp, Warning, TEXT("NewSpeed %f"), NewSpeed);

		if (FMath::IsNearlyEqual(NewSpeed, ClientsSpeed, MaxSpeed * ServerErrorPercent))
		{
			FVector Location = FVector(Value, 0.f, 0.f) * NewSpeed;

			AddActorLocalOffset(Location, true, 0, ETeleportType::TeleportPhysics);

			PlayerLocation = GetActorLocation();

			OnRep_PlayerLocation();
		}
	}
}

void ATankPawn::ServerUpdateRotation_Implementation(float Value)
{
	if (Value != 0 && Value <= 1)
	{
		FRotator Rotation = FRotator(0.f, Value, 0.f);

		AddActorLocalRotation(Rotation, true, 0, ETeleportType::TeleportPhysics);

		PlayerRotation = GetActorRotation();

		OnRep_PlayerRotation();
	}
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"
#include "HealthComponent.h"
#include "TowerOffenseGameModeBase.h"
#include "Components/AudioComponent.h"
#include <Net/UnrealNetwork.h>
#include "BaseHUD.h"
#include "BasePlayerController.h"
#include "Perception/PawnSensingComponent.h"
#include "PVPGameMode.h"

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DefaultRoot"));
	SetRootComponent(CapsuleComponent);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeseMesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(CapsuleComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HelthComponent"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ATurretPawn::HealthChanged);
	//TurretRotationSound1->Stop();
}

// Called every frame
void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurretPawn::SetTeamColor(const FName& ParamName, const FColor& Color, UStaticMeshComponent* Mesh)
{
	int32 MaterialIndex = Mesh->GetMaterialIndex(MaterialSlotName);
	UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateDynamicMaterialInstance(MaterialIndex);

	DynamicMaterial->SetVectorParameterValue(ParamName, Color);

}

TArray<FString> ATurretPawn::GetMaterialNames() const
{
	TArray<FString> Names;
	TArray<UMeshComponent*> MeshComponents;
	GetComponents(MeshComponents, true);
	
	for (const UMeshComponent* MeshComponent : MeshComponents)
	{
		for (const FName& Name : MeshComponent->GetMaterialSlotNames())
		{
			Names.AddUnique(Name.ToString());	
		}
	}
	return Names;
}

/*void ATurretPawn::Server_RotateTurret_Implementation(const FVector& TargetPosition)
{
	MulticastRPCRotateTurret_Implementation(TargetPosition);
}*/

/*bool Server_RotateTurret_Validate(const FVector& TargetPosition)
{
	return true;
}*/

void ATurretPawn::RotateTurret(const FVector& TargetPosition)
{
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator StartRotation = TurretMesh->GetComponentRotation();

	FVector TurretDirection = TargetPosition - StartLocation;
	TurretDirection.Z = 0.f;
	FRotator Rotation = TurretDirection.Rotation();
	Rotation.Normalize();

	FRotator TurretRotation = FMath::RInterpConstantTo(StartRotation, Rotation, 0.2f, InterpRotateSpeed);
	
	TurretMesh->SetWorldRotation(TurretRotation);
}

/*void ATurretPawn::Shoot(const bool& bIsFire)
{
	float TimeBeetweenShots = 60 / NumberOfShotsPerMinute;
	GetWorldTimerManager().SetTimer(ShootRateHandle, this, &ATurretPawn::ShootRate, TimeBeetweenShots, bIsFire, 0.5);
}*/

void ATurretPawn::TurretShoot()
{
	FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	UE_LOG(LogTemp, Warning, TEXT("Turret Fire!"));

	if (!ProjectileClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, SpawnParams);

	if (!Projectile)
	{
		return;
	}

	float DirectionError = FMath::RandRange(-TurretScatter, TurretScatter);
	//ProjectileSpawnRotation.Yaw += DirectionError;
	FVector LaunchDirection = ProjectileSpawnRotation.Vector() + DirectionError;
	
	Projectile->FireDirection(LaunchDirection);

	if (!IsNetMode(NM_DedicatedServer))
	{
		if (!MuzzleFlash)
		{
			return;
		}
	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, ProjectileSpawnLocation);

		if (ShotSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ShotSound, GetActorLocation());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Turret Fire!"));
}

void ATurretPawn::HealthChanged(float NewHealth, float Delta)
{
	if (NewHealth > 0)
	{
		return;
	}
	
	if (!IsNetMode(NM_DedicatedServer))
	{
		if (ExplosivePartical)
		{
			FVector OwnerLocation = GetActorLocation(); UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosivePartical, GetActorLocation());
		}

		if (ExplosiveSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosiveSound, GetActorLocation());
		}
	}
	
	ATowerOffenseGameModeBase* GameModeBase = Cast<ATowerOffenseGameModeBase>(GetWorld()->GetAuthGameMode());
	
	APVPGameMode* PVPGameMode = Cast<APVPGameMode>(GetWorld()->GetAuthGameMode());
	
	if (GameModeBase)
	{
		GameModeBase->HandlePawnKilled(this);
		UE_LOG(LogTemp, Warning, TEXT("Client GameMode"))
	}

	if (PVPGameMode)
	{
		PVPGameMode->HandleKilledTeam(this);
		UE_LOG(LogTemp, Warning, TEXT("Server GameMode"));
	}

	Destroy();
}

UHealthComponent* ATurretPawn::GetHealthComponentOwner_Implementation()
{
	return HealthComponent;
}


//second variant 
/*TArray<FString> ATurretPawn::GetMaterialNames() const
{
	TArray<FName> Names;
	TArray<UMeshComponent*> MeshComponents;
	GetComponents(MeshComponents, true);

	for (const UMeshComponent* MeshComponent : MeshComponents)
	{
		Names.Append(MeshComponent->GetMaterialSlotNames());	
	}

	TArray<FString> NamesStr;
	Algo::Transform(Names, NamesStr, [](const FName& Name)
		{
			return Name.ToString();
		}
	);

	return NamesStr;
}*/
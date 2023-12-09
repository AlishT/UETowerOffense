// Copyright Epic Games, Inc. All Rights Reserved.


#include "TowerOffenseGameModeBase.h"
#include "TurretPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "BasePlayerController.h"
#include "TankPlayerState.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include <Net/UnrealNetwork.h>

void ATowerOffenseGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if(!EnemyPanwClass)
	{
		return;
	}

	TArray<AActor*> AvailablePawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyPanwClass, AvailablePawns);

	EnemyPawns = AvailablePawns.Num();
}

void ATowerOffenseGameModeBase::HandlePawnKilled(ATurretPawn* DeadPawn)
{
	ABasePlayerController* DeadPawnController = Cast<ABasePlayerController>(DeadPawn->GetController());

	if (IsValid(DeadPawnController))
	{
		CurrentPlayerCount--;

		if (CurrentPlayerCount <= 0)
		{
			DeadPawnController->HandleEndGame(false);
		}

		if (!IsNetMode(NM_DedicatedServer))
		{
			ABaseHUD* BaseHUD = Cast<ABaseHUD>(PlayerController->GetHUD());

			BaseHUD->bIsWidgetsHidden = false;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Controller test2 %d"), PlayerControllerList.Num());
	
	if (IsValid(PlayerController))
	{
		PlayerController->ClientPlayCameraShake(CameraShakeClass);

		if (DeadPawn->IsA(EnemyPanwClass))
		{
			EnemyPawns--;
			UE_LOG(LogTemp, Warning, TEXT("Count: %i"), EnemyPawns);
		}

		if (EnemyPawns <= 0)
		{
			PlayerController->HandleEndGame(true);
		}
	}
}

void ATowerOffenseGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Controller %d"), PlayerControllerList.Num());

	if (HasAuthority())
	{

		PlayerController = Cast<ABasePlayerController>(NewPlayer);
		UE_LOG(LogTemp, Warning, TEXT("Controller %s"), *GetNameSafe(PlayerController));

		CurrentPlayerCount = PlayerControllerList.Num();

		/*
		if (MaxPlayersToStart >= GetWorld()->GetGameState()->PlayerArray.Num())
		{
			//ATankPlayerState* PlayerState = Cast<ATankPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
			AGameStateBase* GameState2 = GetWorld()->GetGameState();
			if (GameState2)
			{
				UE_LOG(LogTemp, Warning, TEXT("Controller %d"), GameState2->PlayerArray.Num());
				for (APlayerState* PlayerState : GameState2->PlayerArray)
				{
					// Ignore local player controllers we would have found in the previous pass
					APlayerController* PC = PlayerState ? PlayerState->GetPlayerController() : nullptr;
					if (PC)
					{
						PlayerController = Cast<ABasePlayerController>(PC);

					}
					UE_LOG(LogTemp, Warning, TEXT("Controller %s"), *GetNameSafe(PC));
				}
			}
		}
		//PlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(this, 0));*/
	}
}


/*
void ATowerOffenseGameModeBase::OnRep_SetEnemyPawns(int32 Enemies)
{
	EnemyPawns = Enemies;
}

/*void ATowerOffenseGameModeBase::HandlePawnDied(ATurretPawn* DeadPawn)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(DeadPawn);
	ATowerPawn* EnemyPawn = Cast<ATowerPawn>(DeadPawn);
	int32 EnemyPawns = GetAvailablePawns(EnemyPawn);
	//int32 PlayerPawns = GetAvailablePawns(PlayerPawnClass);
	UE_LOG(LogTemp, Warning, TEXT("Count: %i"), EnemyPawn);

	ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(PlayerPawn->GetController());

	if (BasePlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("BasePlayerController is not nullptr"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BasePlayerController is nullptr"));
	}

	if (IsValid(BasePlayerController))
	{
		BasePlayerController->HandleEndGame(false);
	}
	
	if (IsValid(PlayerPawn))
	{
		if (EnemyPawns <= 0)
		{
			BasePlayerController->HandleEndGame(true);
		}
	}
}*/
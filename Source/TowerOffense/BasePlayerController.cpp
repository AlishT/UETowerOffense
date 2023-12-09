// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "TankPawn.h"
#include "InputMappingContext.h"
#include "PVPGameMode.h"
#include <Net/UnrealNetwork.h>
#include "TeamsSettings.h"
#include "EngineUtils.h"
#include "TeamCheat.h"

void ABasePlayerController::SetPlayerEnabledState()
{
}

ABasePlayerController::ABasePlayerController()
{
	CheatClass = UTeamCheat::StaticClass();
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		Subsystem->ClearAllMappings();
	}
}

void ABasePlayerController::HandleEndGame(bool bIsWin)
{
	if (IsLocalController())
	{
		SetEndGameWidget(bIsWin ? WinWidgetClass : LoseWidgetClass);
		
		if (MoveInputMapping)
		{
			Subsystem->RemoveMappingContext(MoveInputMapping);
		}
	}
}

void ABasePlayerController::SetEndGameWidget(TSubclassOf<UUserWidget> WidgetClass) const
{
	
	if (!WidgetClass) 
	{
		return;
	}
	
	UUserWidget* EndGameWidget = CreateWidget(GetGameInstance(), WidgetClass);
	EndGameWidget->AddToViewport();
}

void ABasePlayerController::SetTimerWidget()
{
	StartTimerWidget = CreateWidget(GetGameInstance(), StartTimerWidgetClass);
	StartTimerWidget->AddToViewport();
}

void ABasePlayerController::EnableInputMappingAndRemoveWidget() const
{
	if (IsLocalController())
	{
		if (MoveInputMapping)
		{
			Subsystem->AddMappingContext(MoveInputMapping, 0);

			StartTimerWidget->RemoveFromParent();
		}
	}
}

void ABasePlayerController::OnStartGame()
{
	GetWorldTimerManager().SetTimer(StartTimerHandle, this, &ABasePlayerController::ReduceTimeStart, 1.f, true, 0.f);

	SetTimerWidget();

	if (!IsNetMode(NM_DedicatedServer))
	{
		UGameplayStatics::PlaySound2D(this, StartSound);
	}
}

void ABasePlayerController::ReduceTimeStart()
{
	OnStartGameTime.Broadcast(StartTime--);

	if (StartTime < 0)
	{
		GetWorldTimerManager().ClearTimer(StartTimerHandle);
		EnableInputMappingAndRemoveWidget();
	}
}

void ABasePlayerController::SetTeamColor(int32 InTeamID, ATankPawn* PlayerPawn)
{
	if (PlayerPawn)
	{
		PlayerPawn->TeamColor = GetDefault<UTeamsSettings>()->Teams[InTeamID];
	}
}

void ABasePlayerController::SetPlayerStart(int32 InTeamID, ATankPawn* PlayerPawn)
{
	APVPGameMode* PVPGameMode = GetWorld()->GetAuthGameMode<APVPGameMode>();

	if (PVPGameMode)
	{
		FVector PlayerStarlLocation = PVPGameMode->TakePlayerStart(InTeamID);
		PlayerPawn->SetActorLocation(PlayerStarlLocation);
	}
}

void ABasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ABasePlayerController, TeamID, Params);
}


void ABasePlayerController::OnRep_TeamID()
{
	ATankPawn* PlayerTankPawn = Cast<ATankPawn>(GetPawn());

	if (PlayerTankPawn)
	{
		SetTeamColor(TeamID, PlayerTankPawn);
		SetPlayerStart(TeamID, PlayerTankPawn);
		PlayerTankPawn->UpdateTeamColor();
	}
}

void ABasePlayerController::ChangeTeam_Implementation(int32 InTeamID)
{
	TeamID = InTeamID;
	MARK_PROPERTY_DIRTY_FROM_NAME(ABasePlayerController, TeamID, this);

	OnRep_TeamID();
}

void ABasePlayerController::AddCheats(bool bForce)
{
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else
	Super::AddCheats(bForce);
#endif
}


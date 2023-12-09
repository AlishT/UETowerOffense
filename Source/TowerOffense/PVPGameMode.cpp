// Fill out your copyright notice in the Description page of Project Settings.


#include "PVPGameMode.h"
#include "BasePlayerController.h"
#include "TankPawn.h"
#include "TeamPlayersStart.h"
#include "EngineUtils.h"
#include <Net/UnrealNetwork.h>
#include "TeamsSettings.h"

void APVPGameMode::HandleKilledTeam(APawn* DeadPlayer)
{
	if (DeadPlayer->GetController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Lose!"));
		/*
		for (auto Team : Loby)
		{
			if (Team.Value == DeadPlayer->GetController())
			{
				//Player--
				UE_LOG(LogTemp, Warning, TEXT("Player Lose!"));
			}
		}*/
	}
}

void APVPGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	MaxTeams = GetDefault<UTeamsSettings>()->Teams.Num();

	CurrentTeamID = FMath::Clamp(CurrentTeamID, 0, MaxTeams);

	if (!NewPlayer)
	{
		return;
	}

	CurrentPlayer = Cast<ABasePlayerController>(NewPlayer);

	if (CurrentPlayer)
	{
		CurrentPlayer->ChangeTeam(CurrentTeamID);
	}

	CurrentTeamID++;
	PlayerCount++;

	if (CurrentTeamID >= MaxTeams && PlayerCount <= MaxPlayers)
	{
		CurrentTeamID = 0;
	}
}

FVector APVPGameMode::TakePlayerStart(int32 TeamID)
{
	for (TActorIterator<ATeamPlayersStart> StartIt(GetWorld()); StartIt; ++StartIt)
	{
		if (TeamID == StartIt->TeamStartID)
		{
			if (StartIt->bEmpty)
			{
				FVector StartLocation = StartIt->GetActorLocation();
				StartIt->bEmpty = false;
				return StartLocation;
			}
		}
	}

	return FVector::ZeroVector;
}


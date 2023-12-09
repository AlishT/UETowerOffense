// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamCheat.h"
#include "BasePlayerController.h"

void UTeamCheat::SetNewTeam(int32 InTeamID)
{
	UE_LOG(LogTemp, Warning, TEXT("Team New Number! %i"), InTeamID);
	
	ABasePlayerController* PlayerT = Cast<ABasePlayerController>(GetPlayerController());
	PlayerT->ChangeTeam(InTeamID);
	
}

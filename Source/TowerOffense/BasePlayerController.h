// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class ATankPawn;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartGameTime, float, Time);

UCLASS()
class TOWEROFFENSE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> LoseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "InputMappingContext")
	UInputMappingContext* MoveInputMapping = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> StartTimerWidgetClass;

	UPROPERTY(Transient)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;
	
	UPROPERTY(Transient)
	UUserWidget* StartTimerWidget = nullptr;

	FTimerHandle StartTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float StartTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* StartSound = nullptr;

public:

	UPROPERTY(BlueprintAssignable)
	FOnStartGameTime OnStartGameTime;
	
	UPROPERTY(ReplicatedUsing = "OnRep_TeamID")
	int32 TeamID = 0;

	FColor TeamColor;

	UFUNCTION()
	void SetPlayerEnabledState();

	UFUNCTION()
	void SetEndGameWidget(TSubclassOf<UUserWidget> WidgetClass) const;

	UFUNCTION()
	void HandleEndGame(bool bIsWin);

	UFUNCTION()
	void SetTimerWidget();

	UFUNCTION()
	void EnableInputMappingAndRemoveWidget() const;

	void OnStartGame();

	UFUNCTION()
	void ReduceTimeStart();
	
	void SetTeamColor(int32 InTeamID, ATankPawn* PlayerPawn);

	UFUNCTION(Server, reliable)
	void ChangeTeam(int32 InTeamID);

	UFUNCTION()
	void OnRep_TeamID();

	void SetPlayerStart(int32 InTeamID, ATankPawn* PlayerPawn);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void AddCheats(bool bForce) override;
	
};

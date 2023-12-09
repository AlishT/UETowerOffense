// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UParticleSystem;
class UWidgetComponent;
class UHealthBarWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWEROFFENSE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float MaxHealth = 100.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_UpdateHealth)
	float CurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* DamageSound = nullptr;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(Server, reliable)
	void Server_GetCurrentHealth(float Damage, float Health);

	UFUNCTION(NetMulticast, reliable)
	void RPCGetCurrentHealth(float Damage);

	UFUNCTION()
	void OnRep_UpdateHealth(float Health);

};

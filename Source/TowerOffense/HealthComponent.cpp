// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "HealthComponentOwner.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	//HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = GetMaxHealth();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		CurrentHealth -= Damage;

		if (DamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
		}

		OnHealthChanged.Broadcast(CurrentHealth, -Damage);

		Server_GetCurrentHealth(Damage, CurrentHealth);
	}

	//HealthBar->SetPlayerHelth(CurrentHealth, GetMaxHealth());

}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::Server_GetCurrentHealth_Implementation(float Damage, float Health)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		CurrentHealth -= Damage;

		if (CurrentHealth != Health)
		{
			OnRep_UpdateHealth(CurrentHealth);
		}
		RPCGetCurrentHealth(Damage);

		OnHealthChanged.Broadcast(CurrentHealth, -Damage);
	}
}

void UHealthComponent::RPCGetCurrentHealth_Implementation(float Damage)
{
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		CurrentHealth -= Damage;

		if (!IsNetMode(NM_DedicatedServer))
		{
			if (DamageSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
			}
		}

		OnHealthChanged.Broadcast(CurrentHealth, -Damage);
	}
}

void UHealthComponent::OnRep_UpdateHealth(float Health)
{
	CurrentHealth = Health;
}



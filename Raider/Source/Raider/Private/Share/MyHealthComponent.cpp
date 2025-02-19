// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize health
	MaxHealth = 100;
	Health = MaxHealth;
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMyHealthComponent::TakeHealing(const float HealAmount)
{
	Health += HealAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UMyHealthComponent::TakeDamage(const float DamageAmount)
{
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Died!"));
	}
}

bool UMyHealthComponent::IsAlive() const
{
	return Health > 0;
}


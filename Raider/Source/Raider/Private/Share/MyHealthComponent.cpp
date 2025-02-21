// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MyHealthComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Share/Struct/FSDamageInfo.h"

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

void UMyHealthComponent::TakeDamage(const float Amount)
{
	if (!IsAlive())
	{
		return;
	}

	// Take damage
	Health -= Amount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	// Handle death if health reaches zero
	if (!IsAlive())
	{
		FOnDeathEvent.Broadcast();
		HandleDeath();
	}
	return;
}

bool UMyHealthComponent::IsAlive() const
{
	return Health > 0;
}

void UMyHealthComponent::HandleDeath() const
{
	if (AActor* Owner = GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *Owner->GetName());

		if (const ACharacter* Character = Cast<ACharacter>(Owner))
		{
			Character->GetCharacterMovement()->DisableMovement();

			PlayDeathMontage(DeathMontage);
		}

		// Destroy the actor after a delay
		Owner->SetLifeSpan(10.0f);
	}
}

void UMyHealthComponent::PlayDeathMontage(UAnimMontage* AnimMontage) const
{
	if (!AnimMontage)
	{
		return;
	}

	if (const AActor* Owner = GetOwner())
	{
		if (const USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AnimMontage);
			}
		}
	}
}


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
	if (!IsAlive())
	{
		return;
	}
	
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
	}
}

bool UMyHealthComponent::IsAlive() const
{
	return Health > 0;
}

void UMyHealthComponent::PlayDeathMontage(UAnimMontage* AnimMontage) const
{
	if (const AActor* Owner = GetOwner())
	{
		if (USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AnimMontage);
			}
		}
	}
}

void UMyHealthComponent::PlayDeathRagDoll() const
{
	if (const AActor* Owner = GetOwner())
	{
		if (USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			MeshComponent->SetSimulatePhysics(true);
			MeshComponent->WakeAllRigidBodies();
			MeshComponent->SetCollisionProfileName("Ragdoll");
		}
	}
}



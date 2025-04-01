// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
	: Health(100),
      MaxHealth(100),
      AttackTokenCount(1)
{
	PrimaryComponentTick.bCanEverTick = false;
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
		OnDeath.Broadcast();
	}
}

bool UMyHealthComponent::IsAlive() const
{
	return Health > 0;
}

bool UMyHealthComponent::RequestAttackToken(AActor* RequestingAttacker, const int32 Amount)
{
	if (!RequestingAttacker)
		return false;

	// If Requesting attacker has token already, return true to prevent token loss
	if (Attackers.Contains(RequestingAttacker))
		return true;

	// Prevent exceeding available tokens
	if (AttackTokenCount < Amount)
		return false;
	
	// Assign token
	AttackTokenCount -= Amount;
	Attackers.Add(RequestingAttacker);
	//UE_LOG(LogTemp, Warning, TEXT("%s give token to %s"), *GetOwner()->GetName(), *RequestingAttacker->GetName());
	
	return true;
}

void UMyHealthComponent::ReturnAttackToken(AActor* RequestingAttacker, const int32 Amount)
{
	if (Attackers.Contains(RequestingAttacker))
	{
		AttackTokenCount += Amount;
		Attackers.Remove(RequestingAttacker);
		//UE_LOG(LogTemp, Warning, TEXT("%s received token from %s"), *GetOwner()->GetName(), *RequestingAttacker->GetName());
	}
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



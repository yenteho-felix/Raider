// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "NPC/NPCCharacterBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NPC/NPCAIController.h"
#include "NPC/Enums/ECharacterMovementState.h"
#include "Share/MyCombatComponent.h"
#include "Share/MyHealthComponent.h"

// Sets default values
ANPCCharacterBase::ANPCCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize movement speeds
	WalkSpeed = 200.0f;
	RunSpeed = 500.0f;
	
	float CurrentMaxSpeed = GetCharacterMovement()->GetMaxSpeed();
	MovementSpeeds.Add(ECharacterMovementState::Idle, 0.0f);
	MovementSpeeds.Add(ECharacterMovementState::Walking, WalkSpeed);
	MovementSpeeds.Add(ECharacterMovementState::Running, RunSpeed);

	// Combat component
	CombatComponent = CreateDefaultSubobject<UMyCombatComponent>("CombatComponent");
	CombatComponent->FOnDamageReactEvent.AddDynamic(this, &ANPCCharacterBase::OnDamageReactEventHandler);
	CombatComponent->FOnDamageBlockedEvent.AddDynamic(this, &ANPCCharacterBase::OnDamageBlockedEventHandler);
	CombatComponent->FOnTakeHitEndEvent.AddDynamic(this, &ANPCCharacterBase::OnTakeHitEndEventHandler);

	// Health component
	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>("HealthComponent");
	HealthComponent->FOnDeathEvent.AddDynamic(this, &ANPCCharacterBase::OnDeathEventHandler);
}

// Called when the game starts or when spawned
void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPCCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ANPCCharacterBase::IsWeaponEquipped_Implementation()
{
	if (!CombatComponent)
	{
		return false;
	}

	return CombatComponent->IsWeaponEquipped;
}

void ANPCCharacterBase::Attack_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->Attack();
	}
}

void ANPCCharacterBase::EquipWeapon_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon();
	}
}

void ANPCCharacterBase::UnEquipWeapon_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->UnEquipWeapon();
	}
}

void ANPCCharacterBase::GetCombatRange_Implementation(float& OutAttackRadius, float& OutDefendRadius)
{
	if (CombatComponent)
	{
		CombatComponent->GetCombatRange(OutAttackRadius, OutDefendRadius);
	}
}

void ANPCCharacterBase::OnDamageReactEventHandler_Implementation(EDamageReact DamageReaction)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage reaction %d received"), static_cast<uint8>(DamageReaction));
	
	// Stop movement
	GetCharacterMovement()->StopMovementImmediately();

	// Update behavior tree to frozen
	if (const ANPCAIController* AIController = Cast<ANPCAIController>(GetInstigatorController()))
	{
		AIController->SetStateAsFrozen();
	}

	// Reaction type specific
	if (DamageReaction == EDamageReact::Hit)
	{
		if (!CombatComponent)
		{
			return;
		}
		CombatComponent->TakeHit();
	}
}

void ANPCCharacterBase::OnTakeHitEndEventHandler()
{
	// Update behavior tree to attacking
	if (ANPCAIController* AIController = Cast<ANPCAIController>(GetInstigatorController()))
	{
		AActor* Target = AIController->AttackTarget;
		AIController->SetStateAsAttacking(Target);
	}
}

void ANPCCharacterBase::OnDamageBlockedEventHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Damage blocked"));
}

float ANPCCharacterBase::GetCurrentHealth_Implementation()
{
	if (!HealthComponent)
	{
		return 0.0f;
	}
	return HealthComponent->Health;
}

float ANPCCharacterBase::GetMaxHealth_Implementation()
{
	if (!HealthComponent)
	{
		return 0.0f;
	}
	return HealthComponent->MaxHealth;
}

void ANPCCharacterBase::TakeHealing_Implementation(const float Amount)
{
	if (!HealthComponent)
	{
		return;
	}
	HealthComponent->TakeHealing(Amount);
}

bool ANPCCharacterBase::TakeDamage_Implementation(const FSDamageInfo& DamageInfo)
{
	if (!HealthComponent)
	{
		return false;
	}

	// Check if combat logic allows damage to be applied
	if (CombatComponent && !CombatComponent->ShouldProcessDamage(DamageInfo))
	{
		return false;
	}

	// Apply damage to health
	HealthComponent->TakeDamage(DamageInfo.Amount);
	return true;
}

bool ANPCCharacterBase::IsDead_Implementation()
{
	if (!HealthComponent)
	{
		return false;
	}
	return !HealthComponent->IsAlive();
}

void ANPCCharacterBase::OnDeathEventHandler_Implementation()
{
	// Stop AI logic if the actor has an AI controller
	if (const ANPCAIController* AIController = Cast<ANPCAIController>(GetInstigatorController()))
	{
		AIController->SetStateAsDead();
		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic("Character is Dead");
		}
	}

	//Play death animation
	if (HealthComponent)
	{
		if (HealthComponent->DeathMontage)
		{
			HealthComponent->PlayDeathMontage(HealthComponent->DeathMontage);
		}
		else
		{
			HealthComponent->PlayDeathRagDoll();
		}
	}

	// Stop movement & collision
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Destroy the actor after a delay
	Owner->SetLifeSpan(10.0f);
}

AActor* ANPCCharacterBase::GetPatrolRoute_Implementation()
{
	return PatrolRoute;
}

float ANPCCharacterBase::SetMovementSpeed_Implementation(ECharacterMovementState InMovementState)
{
	GetCharacterMovement()->MaxWalkSpeed = GetMovementSpeed(InMovementState);
	
	return GetCharacterMovement()->MaxWalkSpeed;
}

float ANPCCharacterBase::GetMovementSpeed(ECharacterMovementState InMovementState)
{
	return MovementSpeeds.Contains(InMovementState) ? MovementSpeeds[InMovementState] : 0.0f;
}


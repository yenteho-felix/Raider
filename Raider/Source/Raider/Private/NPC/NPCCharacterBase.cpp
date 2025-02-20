// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "NPC/NPCCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
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

	// Health component
	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>("HealthComponent");
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

void ANPCCharacterBase::TakeDamage_Implementation(const float Amount)
{
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->TakeDamage(Amount);
}

bool ANPCCharacterBase::IsDead_Implementation()
{
	if (!HealthComponent)
	{
		return false;
	}

	return !HealthComponent->IsAlive();
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


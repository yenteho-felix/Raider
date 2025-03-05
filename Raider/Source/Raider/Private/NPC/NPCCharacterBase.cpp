// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "NPC/NPCCharacterBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NPC/NPCAIController.h"
#include "NPC/Enums/ECharacterMovementState.h"
#include "Perception/AISense_Damage.h"
#include "Share/MyCombatComponent.h"
#include "Share/MyHealthComponent.h"

// Sets default values
ANPCCharacterBase::ANPCCharacterBase()
	: TeamNumber(1)
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

	if (CombatComponent)
	{
		CombatComponent->OnDamageReact.AddDynamic(this, &ANPCCharacterBase::OnDamageReactHandler);
		CombatComponent->OnDamageBlocked.AddDynamic(this, &ANPCCharacterBase::OnDamageBlockedHandler);
		CombatComponent->OnTakeHitEnd.AddDynamic(this, &ANPCCharacterBase::OnTakeHitEndHandler);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ANPCCharacterBase::OnDeathHandler);
	}
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

int32 ANPCCharacterBase::GetTeamNumber_Implementation()
{
	return TeamNumber;
}

bool ANPCCharacterBase::IsWeaponEquipped_Implementation()
{
	if (!CombatComponent)
	{
		return false;
	}

	return CombatComponent->IsWeaponEquipped;
}

void ANPCCharacterBase::Attack_Implementation(AActor* AttackTarget)
{
	if (!AttackTarget) return;
	
	if (CombatComponent)
	{
		CombatComponent->Attack(AttackTarget);
	}
}

bool ANPCCharacterBase::RequestAttackToken_Implementation(AActor* RequestingAttacker, const int Amount)
{
	if (!HealthComponent)
	{
		return true;
	}
	return HealthComponent->RequestAttackToken(RequestingAttacker, Amount);
}

void ANPCCharacterBase::ReturnAttackToken_Implementation(AActor* RequestingAttacker, const int Amount)
{
	if (HealthComponent)
	{
		HealthComponent->ReturnAttackToken(RequestingAttacker, Amount);
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

void ANPCCharacterBase::OnDamageReactHandler_Implementation(EDamageReact DamageReaction)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s is taking damage of type EDamageReact[%d]"), *GetName(), static_cast<uint8>(DamageReaction));
	
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

void ANPCCharacterBase::OnTakeHitEndHandler()
{
	// Update behavior tree to attacking
	if (ANPCAIController* AIController = Cast<ANPCAIController>(GetInstigatorController()))
	{
		AActor* Target = AIController->AttackTarget;
		AIController->SetStateAsAttacking(Target);
	}
}

void ANPCCharacterBase::OnDamageBlockedHandler()
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

bool ANPCCharacterBase::TakeDamage_Implementation(AActor* Attacker, const FSDamageInfo& DamageInfo)
{
	if (!HealthComponent || !CombatComponent)
	{
		return false;
	}
	
	// Check if combat logic allows damage to be applied
	if (CombatComponent->ShouldProcessDamage(DamageInfo))
	{
		// Apply damage to health
		HealthComponent->TakeDamage(DamageInfo.Amount);

		// Report damage event to AI perception system
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			GetOwner(),
			Attacker,
			DamageInfo.Amount,
			GetOwner()->GetActorLocation(),
			Attacker->GetActorLocation()
		);
		
		return true;
	}

	return false;
}

bool ANPCCharacterBase::IsDead_Implementation()
{
	if (!HealthComponent)
	{
		return false;
	}
	return !HealthComponent->IsAlive();
}

void ANPCCharacterBase::OnDeathHandler_Implementation()
{
	// Return attack token
	if (CombatComponent && CombatComponent->CurrentAttackTarget)
	{
		AActor* AttackTarget = CombatComponent->CurrentAttackTarget;
		if (AttackTarget->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
		{
			IMyCombatInterface::Execute_ReturnAttackToken(AttackTarget, this, 1);
		}
	}
	
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


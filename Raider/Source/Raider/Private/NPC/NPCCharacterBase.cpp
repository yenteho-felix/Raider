// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "NPC/NPCCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NPC/NPCAIController.h"
#include "NPC/Enums/ECharacterMovementState.h"

// Sets default values
ANPCCharacterBase::ANPCCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize movement speeds
	float CurrentMaxSpeed = GetCharacterMovement()->GetMaxSpeed();
	MovementSpeeds.Add(ECharacterMovementState::Idle, 0.0f);
	MovementSpeeds.Add(ECharacterMovementState::Walking, 200.0f);
	MovementSpeeds.Add(ECharacterMovementState::Running, 500.0f);
}

// Called when the game starts or when spawned
void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Get AI controller and assign its behavior tree
	SetupDynamicBehaviorTreeAsset();
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

void ANPCCharacterBase::SetupDynamicBehaviorTreeAsset() const
{
	ANPCAIController* MyAIController = Cast<ANPCAIController>(GetController());
	if (MyAIController && BehaviorTreeAsset)
	{
		MyAIController->BehaviorTreeAsset = BehaviorTreeAsset;
	}
}

void ANPCCharacterBase::TriggerOnEquipWeaponEnd()
{
	OnEquipWeaponEnd.Broadcast();
}

void ANPCCharacterBase::TriggerOnUnEquipWeaponEnd()
{
	OnUnEquipWeaponEnd.Broadcast();
}

void ANPCCharacterBase::TriggerOnAttackEnd()
{
	OnAttackEnd.Broadcast();
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


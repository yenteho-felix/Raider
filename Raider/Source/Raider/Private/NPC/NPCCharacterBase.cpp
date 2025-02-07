// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "NPC/NPCCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANPCCharacterBase::ANPCCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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


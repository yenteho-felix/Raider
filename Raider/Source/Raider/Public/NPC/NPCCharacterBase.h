// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Share/MyCombatInterface.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

class UMyCombatComponent;
class UMyHealthComponent;
class UBehaviorTree;

/**
 *	========================================================
 *  A base actor class for NPCs in the game
 *  ========================================================
 */
UCLASS()
class RAIDER_API ANPCCharacterBase : public ACharacter, public IMyCombatInterface
{
	GENERATED_BODY()

public:
	ANPCCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/**
 *	---------------------------------------------
 *  Behavior Tree
 *  ---------------------------------------------
 */
public:
	/** Each AI Character type can assign a unique behavior tree, which can be retrieved from AI Controller */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AIController")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
/**
 *	---------------------------------------------
 *  Combat Component & Interface
 *  ---------------------------------------------
 */
public:
	/** Component handling NPC combat-related functionalities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	UMyCombatComponent* CombatComponent;

	/**
	 *  NPCCombatInterface, check if currently has a weapon equipped.
	 *  @return True if a weapon is equipped, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual bool IsWeaponEquipped_Implementation() override;
	
	/** NPCCombatInterface, attack function */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual void Attack_Implementation() override;

	/** NPCCombatInterface, equip function */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual void EquipWeapon_Implementation() override;

	/** NPCCombatInterface, UnEquip function */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual void UnEquipWeapon_Implementation() override;

	/**
	 *  NPCCombatInterface, retrieve combat range
	 *  @param OutAttackRadius - The attack range of the NPC
	 *  @param OutDefendRadius - The defensive range of the NPC
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual void GetCombatRange_Implementation(float& OutAttackRadius, float& OutDefendRadius) override;
	
/**
 *	---------------------------------------------
 *  Health Component & Interface
 *  ---------------------------------------------
 */
public:
	/** Component handling NPC health-related functionalities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	UMyHealthComponent* HealthComponent;

	/** Retrieves the NPC's current health value */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual float GetCurrentHealth_Implementation() override;

	/** Retrieves the NPC's maximum health value */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual float GetMaxHealth_Implementation() override;

	/**
	 *  Restores the NPC's health by a specified amount
	 *  @param Amount - The amount of health to restore
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual void TakeHealing_Implementation(float Amount) override;

	/**
	 *  Applies damage to the NPC, reducing its current health
	 *  @param Amount - The amount of damage taken
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual void TakeDamage_Implementation(float Amount) override;

	/** Checks if the NPC is dead */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	virtual bool IsDead_Implementation() override;
	
/**
 *	---------------------------------------------
 *  Movement & Interface
 *  ---------------------------------------------
 */
public:
	/** The patrol route object that defines NPC patrol movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface")
	TObjectPtr<AActor> PatrolRoute;

	/** Walking speed of the NPC */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface|Movement")
	float WalkSpeed;

	/** Running speed of the NPC */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface|Movement")
	float RunSpeed;
	
protected:
	/** NPCCombatInterface, retrieves the patrol route assigned to the NPC */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	AActor* GetPatrolRoute_Implementation() override;

	/** Mapping from movement states to their corresponding movement speeds */
	TMap<ECharacterMovementState, float> MovementSpeeds;
	
	/**
	 *  Sets the NPC's movement speed based on the given movement state
	 *  @param InMovementState - The movement state to set
	 *  @return The corresponding movement speed for the state
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	float SetMovementSpeed_Implementation(ECharacterMovementState InMovementState) override;

private:
	/**
	 *  Retrieves the movement speed for a given movement state
	 *  @param InMovementState - The movement state to query
	 *  @return The corresponding movement speed
	 */
	float GetMovementSpeed(ECharacterMovementState InMovementState);
};

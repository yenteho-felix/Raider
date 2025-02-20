// Game Development Exercise by Felix Ho

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPC/Enums/ECharacterMovementState.h"
#include "NPCCombatInterface.generated.h"

class UMyCombatComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPCCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	=========================================================================
 *  A Combat interface to hold combat related functions that NPC needs
 *  =========================================================================
 */
class RAIDER_API INPCCombatInterface
{
	GENERATED_BODY()
	
/**
 *	------------------------------	
 *  Battle
 *	------------------------------	
 */
public:
	/**
	 *	Checks if the NPC currently has a weapon equipped.
	 *  @return True if a weapon is equipped, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsWeaponEquipped();
	
	/** Called when the NPC equip a weapon. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipWeapon();

	/** Called when the NPC un-equip a weapon. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnEquipWeapon();

	/** Called when the NPC performs an attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();

	/**
	 *  Retrieves the combat range of the NPC.
	 *  @param AttackRadius - The distance within which the NPC can attack.
	 *  @param DefendRadius - The distance within which the NPC will consider defending.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetCombatRange(float& AttackRadius, float& DefendRadius);

/**
 *	------------------------------
 *  Health
 *	------------------------------	
 */
public:
	/** 
	 *  Retrieves the current health value of the NPC.
	 *  @return The current health as a float.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetCurrentHealth();
	
	/** 
	 *  Retrieves the maximum possible health of the NPC.
	 *  @return The max health as a float.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaxHealth();

	/** 
	 *  Restores health based on a given percentage of max health.
	 *  @param Amount - The amount of health to add.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeHealing(float Amount);

	/** 
	 *  Applies damage to the NPC, reducing its current health.
	 *  @param Amount - The amount of health to subtract.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(float Amount);

	/** 
	 *  Checks if the NPC is dead (health is zero or below).
	 *  @return True if the NPC has zero or negative health, otherwise false.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead();
	
/**
 *	------------------------------
 *  Movement
 *	------------------------------	
 */
public:
	/**
	 *  Retrieves the patrol route assigned to the NPC.
	 *  @return A reference to an AActor that defines the patrol route.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetPatrolRoute();
	
	/**
	 *  Sets the NPC's movement speed based on the given movement state.
	 *  @param InMovementState - The movement state to apply.
	 *  @return The resulting movement speed after applying the state.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float SetMovementSpeed(ECharacterMovementState InMovementState);
};

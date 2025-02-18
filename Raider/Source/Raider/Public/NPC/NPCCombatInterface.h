// Game Development Exercise by Felix Ho

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPC/Enums/ECharacterMovementState.h"
#include "NPCCombatInterface.generated.h"

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
 *  Weapon
 *	------------------------------	
 */
public:
	/** Called when the NPC equips a weapon. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipWeapon();

	/** Called when the NPC unequips a weapon. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnEquipWeapon();

/**
 *	------------------------------	
 *  Combat
 *	------------------------------	
 */
public:
	/** Called when the NPC performs an attack. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();

	/**
	 * Retrieves the combat range of the NPC.
	 * @param AttackRadius - The distance within which the NPC can attack.
	 * @param DefendRadius - The distance within which the NPC will consider defending.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetCombatRange(float& AttackRadius, float& DefendRadius);
	
/**
 *	------------------------------
 *  Health
 *	------------------------------	
 */
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Heal(float Percentage);
	
/**
 *	------------------------------
 *  Movement
 *	------------------------------	
 */
public:
	/**
	 * Retrieves the patrol route assigned to the NPC.
	 * @return A reference to an AActor that defines the patrol route.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetPatrolRoute();
	
	/**
	 * Sets the NPC's movement speed based on the given movement state.
	 * @param InMovementState - The movement state to apply.
	 * @return The resulting movement speed after applying the state.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float SetMovementSpeed(ECharacterMovementState InMovementState);
};

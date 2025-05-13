// Game Development Exercise by Felix Ho

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatSystemAPI.h"
#include "NPC/Enums/ECharacterMovementState.h"
#include "Structs/FSDamageInfo.h"
#include "MyCombatInterface.generated.h"

// Make the interface blueprintable
UINTERFACE(Blueprintable)
class UMyCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	=========================================================================
 *  A Combat interface to hold combat related functions that NPC needs
 *  =========================================================================
 */
class COMBATSYSTEM_API IMyCombatInterface
{
	GENERATED_BODY()

/**
 *  ------------------------------
 *  Team
 *  ------------------------------
 */
public:
	/** Retrieve team number */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetTeamNumber();
	
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
	void Attack(AActor* AttackTarget);

	/** Called when the NPC performs a block action. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Block();

	/**
	 * Attempts to reserve attack tokens for an attacking entity.
	 * @param RequestingAttacker - The entity requesting attack permission.
	 * @param Amount - The number of attack tokens to request.
	 * @return True if tokens were successfully granted, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool RequestAttackToken(AActor* RequestingAttacker, int Amount);

	/**
	 * Releases attack tokens after an attacker disengages.
	 * @param RequestingAttacker - The entity returning the attack token.
	 * @param Amount - The number of tokens being returned.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnAttackToken(AActor* RequestingAttacker, int Amount);
	
	/**
	 *  Retrieves the combat range of the NPC. Add Default C++ implementation.
	 *  @param OutAttackRadius - The distance within which the NPC can attack.
	 *  @param OutDefendRadius - The distance within which the NPC will consider defending.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetCombatRange(float& OutAttackRadius, float& OutDefendRadius);
	virtual void GetCombatRange_Implementation(float& OutAttackRadius, float& OutDefendRadius);

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
	 *  @param DamageInfo - The damage info
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool TakeDamage(AActor* Attacker, const FSDamageInfo& DamageInfo);

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

/**
 *	------------------------------
 *  Delegate Events
 *	------------------------------	
 */
public:
	/** 
	 *  Apply necessary death logic when character is dead 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDeathHandler();

	/** 
	 *  Apply necessary logic when character get damaged 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDamageReactHandler(EDamageReact DamageReaction);
};

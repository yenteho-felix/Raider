// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Share/MyCombatInterface.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

class UMyCombatComponent;
class UMyHealthComponent;
class UBehaviorTree;
struct FSDamageInfo;

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
 *  Team
 *  ---------------------------------------------
 */
	/** NPCCombatInterface, return team number */
	UFUNCTION(Category = "Player")
	virtual int32 GetTeamNumber_Implementation() override;

private:
	/** Default team number */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Team", meta = (AllowPrivateAccess = "true"))
	int32 TeamNumber;
	
/**
 *	---------------------------------------------
 *  Combat Component & Interface
 *  ---------------------------------------------
 */
public:
	/** Component handling NPC combat-related functionalities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	UMyCombatComponent* CombatComponent;

	/** NPCCombatInterface, check if currently has a weapon equipped */
	UFUNCTION(Category = "NPC")
	virtual bool IsWeaponEquipped_Implementation() override;
	
	/** NPCCombatInterface, attack function */
	UFUNCTION(Category = "NPC")
	virtual void Attack_Implementation(AActor* AttackTarget) override;

	/** NPCCombatInterface, request token */
	UFUNCTION(Category = "NPC")
	virtual bool RequestAttackToken_Implementation(AActor* RequestingAttacker, const int Amount) override;
	
	/** NPCCombatInterface, return token */
	UFUNCTION(Category = "NPC")
	virtual void ReturnAttackToken_Implementation(AActor* RequestingAttacker, const int Amount) override;

	/** NPCCombatInterface, equip function */
	UFUNCTION(Category = "NPC")
	virtual void EquipWeapon_Implementation() override;

	/** NPCCombatInterface, UnEquip function */
	UFUNCTION(Category = "NPC")
	virtual void UnEquipWeapon_Implementation() override;

	/** NPCCombatInterface, retrieve combat range */
	UFUNCTION(Category = "NPC")
	virtual void GetCombatRange_Implementation(float& OutAttackRadius, float& OutDefendRadius) override;
	
private:
	/**
	 *  Variable to store current attack target passing from Attack() function
	 */
	UPROPERTY()
	AActor* CurrentAttackTarget;
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
	UFUNCTION(Category = "NPC")
	virtual float GetCurrentHealth_Implementation() override;

	/** Retrieves the NPC's maximum health value */
	UFUNCTION(Category = "NPC")
	virtual float GetMaxHealth_Implementation() override;

	/** Restores the NPC's health by a specified amount */
	UFUNCTION(Category = "NPC")
	virtual void TakeHealing_Implementation(float Amount) override;

	/** Applies damage to the NPC, reducing its current health */
	UFUNCTION(Category = "NPC")
	virtual bool TakeDamage_Implementation(AActor* Attacker, const FSDamageInfo& DamageInfo) override;

	/** Checks if the NPC is dead */
	UFUNCTION(Category = "NPC")
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

	/** NPCCombatInterface, retrieves the patrol route assigned to the NPC */
	UFUNCTION(Category = "NPC|Combat Interface")
	virtual AActor* GetPatrolRoute_Implementation() override;
	
	/** NPCCombatInterface, sets the NPC's movement speed based on the given movement state */
	UFUNCTION(Category = "NPC|Combat Interface")
	virtual float SetMovementSpeed_Implementation(ECharacterMovementState InMovementState) override;
	
protected:
	/** Mapping from movement states to their corresponding movement speeds */
	TMap<ECharacterMovementState, float> MovementSpeeds;

	/**
	 *  Retrieves the movement speed for a given movement state
	 *  @param InMovementState - The movement state to query
	 *  @return The corresponding movement speed
	 */
	float GetMovementSpeed(ECharacterMovementState InMovementState);

/**
 *	---------------------------------------------
 *  Delegate Events
 *  ---------------------------------------------
 */
private:
	/** NPCCombatInterface, activities when taking damage */
	UFUNCTION(Category = "NPC|Delegate")
	virtual void OnDamageReactHandler_Implementation(EDamageReact DamageReaction) override;

	/** Default implementation when NPC character dead */
	UFUNCTION(Category = "NPC|Delegate")
	virtual void OnDeathHandler_Implementation() override;
	
	/** Activities after taking hit ended */
	UFUNCTION(Category = "NPC|Delegate")
	void OnTakeHitEndHandler();

	/** Activities when blocking damage */
	UFUNCTION(Category = "NPC|Delegate")
	void OnDamageBlockedHandler();
};

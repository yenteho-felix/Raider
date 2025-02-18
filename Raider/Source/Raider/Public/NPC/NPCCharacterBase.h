// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCCombatInterface.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

class AWeaponBase;
class UBehaviorTree;

/** Delegate to notify subscribers when weapon equip process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipWeaponEnd);

/** Delegate to notify subscribers when weapon un-equip process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquipWeaponEnd);

/** Delegate to notify subscribers when attack process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

/**
 *	========================================================
 *  A base actor class for NPC used in the game
 *  ========================================================
 */
UCLASS()
class RAIDER_API ANPCCharacterBase : public ACharacter, public INPCCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/**
 *	---------------------------------------------
 *  Behavior Tree
 *  ---------------------------------------------
 */
public:
	/** Each AI Character type can assign a unique behavior tree, which can be retrieved from NPCAIController */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AIController")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
/**
 *	---------------------------------------------
 *  Combat Interface - Weapon
 *  ---------------------------------------------
 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface")
	bool IsWeaponEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface")
	TSubclassOf<AWeaponBase> WeaponActor;
	
	/** Event triggered when weapon equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "NPC|Combat Interface")
	FOnEquipWeaponEnd OnEquipWeaponEnd;

	/** Event triggered when weapon un-equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "NPC|Combat Interface")
	FOnUnEquipWeaponEnd OnUnEquipWeaponEnd;

protected:
	/** Broadcasts the OnEquipWeaponEnd event */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	void TriggerOnEquipWeaponEnd();

	/** Broadcasts the OnUnEquipWeaponEnd event */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	void TriggerOnUnEquipWeaponEnd();

/**
 *	---------------------------------------------
 *  Combat Interface - Combat
 *  ---------------------------------------------
 */
public:
	/** Event triggered when weapon equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "NPC|Combat Interface")
	FOnAttackEnd OnAttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface|Combat")
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface|Combat")
	float DefendRadius;
	
	/** NPCCombatInterface, retrieve combat range */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	void GetCombatRange_Implementation(float& InAttackRadius, float& InDefendRadius) override;

protected:
	/** Broadcasts the OnAttackEnd event */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	void TriggerOnAttackEnd();
	
/**
 *	---------------------------------------------
 *  Combat Interface - Movement
 *  ---------------------------------------------
 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface")
	TObjectPtr<AActor> PatrolRoute;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface|Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Combat Interface|Movement")
	float RunSpeed;
	
protected:
	/** NPCCombatInterface, return patrol route object reference */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	AActor* GetPatrolRoute_Implementation() override;
	
	/** NPCCombatInterface, return float as movement speed */
	UFUNCTION(BlueprintCallable, Category = "NPC|Combat Interface")
	float SetMovementSpeed_Implementation(ECharacterMovementState InMovementState) override;

	/** Define a mapping from movement state to movement speed */
	TMap<ECharacterMovementState, float> MovementSpeeds;

private:
	/** Query the TMap and return speed for given movement state */
	float GetMovementSpeed(ECharacterMovementState InMovementState);
};

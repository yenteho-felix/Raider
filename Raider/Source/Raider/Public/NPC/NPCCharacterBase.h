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
 *  A base actor class for NPC used in the game
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
 *  Behavior Tree
 */
public:
	/** Each AI Character type can assign a unique behavior tree, which can be retrieved from NPCAIController */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AIController")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
/**
 *  Combat Interface
 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|ComcatInterface")
	bool IsWeaponEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|ComcatInterface")
	TSubclassOf<AWeaponBase> WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|ComcatInterface")
	TObjectPtr<AActor> PatrolRoute;
	
	/** Event triggered when weapon equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "NPC|ComcatInterface")
	FOnEquipWeaponEnd OnEquipWeaponEnd;

	/** Event triggered when weapon un-equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "NPC|ComcatInterface")
	FOnEquipWeaponEnd OnUnEquipWeaponEnd;

	/** Event triggered when weapon equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "NPC|ComcatInterface")
	FOnAttackEnd OnAttackEnd;

protected:
	/** Broadcasts the OnEquipWeaponEnd event */
	UFUNCTION(BlueprintCallable, Category = "NPC|ComcatInterface")
	void TriggerOnEquipWeaponEnd();

	/** Broadcasts the OnUnEquipWeaponEnd event */
	UFUNCTION(BlueprintCallable, Category = "NPC|ComcatInterface")
	void TriggerOnUnEquipWeaponEnd();
	
	/** Broadcasts the OnAttackEnd event */
	UFUNCTION(BlueprintCallable, Category = "NPC|ComcatInterface")
	void TriggerOnAttackEnd();
	
	/** NPCCombatInterface, return patrol route object reference */
	UFUNCTION(BlueprintCallable, Category = "NPC|ComcatInterface")
	AActor* GetPatrolRoute_Implementation() override;
	
	/** NPCCombatInterface, return float as movement speed */
	UFUNCTION(BlueprintCallable, Category = "NPC|ComcatInterface")
	float SetMovementSpeed_Implementation(ECharacterMovementState InMovementState) override;

	/** Define a mapping from movement state to movement speed */
	TMap<ECharacterMovementState, float> MovementSpeeds;

private:
	/** Query the TMap and return speed for given movement state */
	float GetMovementSpeed(ECharacterMovementState InMovementState);
};

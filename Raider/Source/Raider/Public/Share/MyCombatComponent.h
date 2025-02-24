// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCombatComponent.generated.h"

struct FSDamageInfo;
class AWeaponBase;

/** Delegate to notify subscribers when weapon equip process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipWeaponEnd);

/** Delegate to notify subscribers when weapon un-equip process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquipWeaponEnd);

/** Delegate to notify subscribers when attack process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

/** Delegate to notify subscribers when character is taking damage */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReact, EDamageReact, DamageReaction);

/** Delegate to notify subscribers when attack process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeHitEnd);

/** Delegate to notify subscribers when character is blocking damage */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageBlocked);

/**
 *  =====================================================
 *  Combat component responsible for handling combat mechanics
 *  =====================================================
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDER_API UMyCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyCombatComponent();

protected:
	virtual void BeginPlay() override;

/**
 *	---------------------------------------------
 *  Weapon
 *  ---------------------------------------------
 */
public:
	/** Whether the NPC currently has a weapon equipped */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	bool IsWeaponEquipped;

	/** The weapon class that can be spawned for the NPC */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TSubclassOf<AWeaponBase> WeaponActorClass;

	/** Reference to the currently equipped weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<AWeaponBase> WeaponActorObj;

	/** Animation montage played when equipping a weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TObjectPtr<UAnimMontage> EquipMontage;

	/** Animation montage played when unequipping a weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TObjectPtr<UAnimMontage> UnEquipMontage;

	/** The socket name where the weapon will be attached on the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	FName WeaponSocketName;

	/** Equips the assigned weapon */
	UFUNCTION(BlueprintCallable, Category = "Combat|Weapon")
	void EquipWeapon();

	/** Unequips the currently equipped weapon */
	UFUNCTION(BlueprintCallable, Category = "Combat|Weapon")
	void UnEquipWeapon();

protected:
	/**
	 *  Attaches the weapon to a specified socket on the character.
	 *  @param WeaponActor - The weapon actor to attach.
	 */
	UFUNCTION()
	void AttachWeaponToSocket(AActor* WeaponActor) const;

	/**
	 *	Plays the equip montage animation.
	 *	@param AnimMontage - The montage to play for equipping the weapon.
	 */
	UFUNCTION()
	void PlayEquipMontage(UAnimMontage* AnimMontage);

	/**
	 *  Plays the unequip montage animation.
	 *  @param AnimMontage - The montage to play for unequipping the weapon.
	 */
	UFUNCTION()
	void PlayUnEquipMontage(UAnimMontage* AnimMontage);
	
public:
	/** Event triggered when weapon equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Weapon")
	FOnEquipWeaponEnd OnEquipWeaponEnd;

	/** Event triggered when weapon un-equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Weapon")
	FOnUnEquipWeaponEnd OnUnEquipWeaponEnd;

protected:
	/**
	 *  Callback function triggered when the equip montage animation finishes.
	 *  Broadcasts the `OnEquipWeaponEnd` event.
	 *  @param Montage - The animation montage that completed.
	 *  @param bInterrupted - Whether the montage was interrupted.
	 */	UFUNCTION()
	void OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;

	/**
	 *  Callback function triggered when the unequip montage animation finishes.
	 *  Broadcasts the `OnUnEquipWeaponEnd` event.
	 *  @param Montage - The animation montage that completed.
	 *  @param bInterrupted - Whether the montage was interrupted.
	 */	UFUNCTION()
	void OnUnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;

/**
 *	---------------------------------------------
 *  Attack
 *  ---------------------------------------------
 */
public:
	/** Animation montage played when the NPC performs an attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** The radius within which the NPC can perform an attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	float AttackRadius;

	/** The radius within which the NPC considers defending */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	float DefendRadius;

	/**
	 *  Retrieves the attack and defense ranges of the NPC.
	 *  @param OutAttackRadius - Output parameter for attack radius.
	 *  @param OutDefendRadius - Output parameter for defend radius.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void GetCombatRange(float& OutAttackRadius, float& OutDefendRadius) const;

	/** Executes the NPC's attack behavior */
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void Attack();

protected:
	/**
	 *  Plays the attack montage animation.
	 *  @param AnimMontage - The montage to play for attacking.
	 */
	UFUNCTION()
	void PlayAttackMontage(UAnimMontage* AnimMontage);
	
public:
	/** Event triggered when attack process ends */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Attack")
	FOnAttackEnd OnAttackEnd;
	
	/**
	 *  Callback function triggered when the attack montage animation finishes.
	 *  @param Montage - The animation montage that completed.
	 *  @param bInterrupted - Whether the montage was interrupted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;

/**
 *	---------------------------------------------
 *  Defense & Response
 *  ---------------------------------------------
 */
public:
	/** Animation montage played when taking hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Defense")
	TObjectPtr<UAnimMontage> TakeHitMontage;
	
	/** Indicates whether the character is invincible */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Defense")
	bool bIsInvincible;
	
	/** Indicate whether the character is blocking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Defense")
	bool bIsBlocking;

	/** Indicate whether the damage can be interrupted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Defense")
	bool bIsInterruptible;
	
	/**
	 *  React to damage
	 *  @param DamageInfo - The damage info
	 *  @Return return true if character is taking damage, otherwise false
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Defense")
	bool ShouldProcessDamage(const FSDamageInfo& DamageInfo) const;

	/** Delegate event triggered when the character is taking damage */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Defense")
	FOnDamageReact FOnDamageReactEvent;

	/** Delegate event triggered when the character is blocking damage */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Defense")
	FOnDamageBlocked FOnDamageBlockedEvent;

	/** Taking hit */
	UFUNCTION(BlueprintCallable, Category = "Combat|Defense")
	void TakeHit();
	
protected:
	/**
	 *  Plays the hit montage animation.
	 *  @param AnimMontage - The montage to play when taking hit
	 */
	UFUNCTION(Category = "Combat|Defense")
	void PlayTakeHitMontage(UAnimMontage* AnimMontage);

public:
	/** Event triggered when taking hit process ends */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Defense")
	FOnTakeHitEnd FOnTakeHitEndEvent;
	
	/**
	 *  Callback function triggered when the taking hit montage animation finishes.
	 *  Broadcasts the `OnTakeHitEnd` event.
	 *  @param Montage - The animation montage that completed.
	 *  @param bInterrupted - Whether the montage was interrupted.
	 */	UFUNCTION(BlueprintCallable, Category = "Combat|Defense")
	void OnTakeHitMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;
};

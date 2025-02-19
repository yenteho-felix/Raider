// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCombatComponent.generated.h"


class AWeaponBase;

/** Delegate to notify subscribers when weapon equip process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipWeaponEnd);

/** Delegate to notify subscribers when weapon un-equip process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquipWeaponEnd);

/** Delegate to notify subscribers when attack process is completed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

/**
 *  =====================================================
 *  Combat component to hold combat related properties and functions
 *  =====================================================
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDER_API UMyCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

/**
 *	---------------------------------------------
 *  Weapon
 *  ---------------------------------------------
 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	bool IsWeaponEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TSubclassOf<AWeaponBase> WeaponActorClass;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<AWeaponBase> WeaponActorObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TObjectPtr<UAnimMontage> EquipMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TObjectPtr<UAnimMontage> UnEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	FName WeaponSocketName;

	UFUNCTION(BlueprintCallable, Category = "Combat|Weapon")
	void EquipWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Combat|Weapon")
	void UnEquipWeapon();

protected:
	UFUNCTION()
	void AttachWeaponToSocket(AActor* WeaponActor) const;

	UFUNCTION()
	void PlayEquipMontage(UAnimMontage* AnimMontage);

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
	/** Callback when OnEquipMontageEnded finishes to broadcast the OnEquipWeaponEnd event */
	UFUNCTION()
	void OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;

	/** Callback when OnUnEquipMontageEnded finishes to broadcast the OnUnEquipWeaponEnd event */
	UFUNCTION()
	void OnUnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;

/**
 *	---------------------------------------------
 *  Attack
 *  ---------------------------------------------
 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	float DefendRadius;
	
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void GetCombatRange(float& OutAttackRadius, float& OutDefendRadius) const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void Attack();

protected:
	UFUNCTION()
	void PlayAttackMontage(UAnimMontage* AnimMontage);
	
public:
	/** Event triggered when weapon equip process ends */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Attack")
	FOnAttackEnd OnAttackEnd;
	
	/** Callback when OnUnAttackMontageEnded finishes to broadcast the OnAttackEnd event */
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;
};

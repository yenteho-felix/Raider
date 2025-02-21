// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

enum class EDamageReact : uint8;
struct FSDamageInfo;

/** Delegate to notify subscribers when character is dead */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

/**
 *  =====================================================
 *  A health management component that handles health-related functionalities
 *  =====================================================
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDER_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Animation montage played when character is dead */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	/** Current health of the entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	/** Maximum health of the entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	/**
	 * Restores health to the entity.
	 * @param HealAmount - The amount of health to restore.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeHealing(float HealAmount);

	/**
	 *  Reduces the entity's health when taking damage.
	 *  @param Amount - The damage amount
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(const float Amount);

	/**
	 * Checks if the entity is still alive.
	 * @return True if health is above zero, otherwise false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

	/**
	 *  Apply necessary death logic when character is dead 
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void HandleDeath() const;

/**
 *  -----------------------------------------
 *  Delegates
 *  -----------------------------------------
 */
public:
	/** Delegate event triggered when the character dies */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath FOnDeathEvent;

/**
 *  -----------------------------------------
 *  Animations
 *  -----------------------------------------
 */	
protected:
	/**
	 *  Plays the death montage animation.
	 *  @param AnimMontage - The montage to play when character is dead
	 */
	UFUNCTION()
	void PlayDeathMontage(UAnimMontage* AnimMontage) const;
};

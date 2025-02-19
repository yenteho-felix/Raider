// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

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
	UFUNCTION(BlueprintCallable)
	void TakeHealing(float HealAmount);

	/**
	 * Reduces the entity's health when taking damage.
	 * @param DamageAmount - The amount of damage to apply.
	 */
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	/**
	 * Checks if the entity is still alive.
	 * @return True if health is above zero, otherwise false.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
};

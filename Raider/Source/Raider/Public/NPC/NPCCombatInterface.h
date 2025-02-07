// Game Development Exercise by Felix Ho

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPCCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  A Combat interface to hold combat related functions that NPC needs
 */
class RAIDER_API INPCCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintNativeEvent)
	void UnEquipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();
};

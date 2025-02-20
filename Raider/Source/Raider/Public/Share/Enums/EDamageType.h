// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 *  Enum for Damage Type
 */
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None			UMETA(DisplayName = "None"),
	Melee			UMETA(DisplayName = "Melee"),
	Projectile		UMETA(DisplayName = "Projectile"),
	Explosion		UMETA(DisplayName = "Explosion"),
	Environment		UMETA(DisplayName = "Environment"),
};

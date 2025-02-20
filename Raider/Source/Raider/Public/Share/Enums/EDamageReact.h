// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 *  Enum for Damage Type
 */
UENUM(BlueprintType)
enum class EDamageReact : uint8
{
	None			UMETA(DisplayName = "None"),
	Hit				UMETA(DisplayName = "Hit"),
	Stagger			UMETA(DisplayName = "Stagger"),
	Stun			UMETA(DisplayName = "Stun"),
	KnockBack		UMETA(DisplayName = "KnockBack"),
	Death			UMETA(DisplayName = "Death")
};

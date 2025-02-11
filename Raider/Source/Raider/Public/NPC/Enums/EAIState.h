// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 *  Enum for AI states
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive			UMETA(DisplayName = "Passive"),
	Attacking		UMETA(DisplayName = "Attacking"),
	Frozen			UMETA(DisplayName = "Frozen"),
	Investigating	UMETA(DisplayName = "Investigating"),
	Dead			UMETA(DisplayName = "Dead"),
};

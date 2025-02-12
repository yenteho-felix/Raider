// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 *  Enum for AI sense
 */
UENUM(BlueprintType)
enum class EAISense : uint8
{
	None			UMETA(DisplayName = "None"),
	Sight			UMETA(DisplayName = "Sight"),
	Hearing			UMETA(DisplayName = "Hearing"),
	Damage			UMETA(DisplayName = "Damage"),
};

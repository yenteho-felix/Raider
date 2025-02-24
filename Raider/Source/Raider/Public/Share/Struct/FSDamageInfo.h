#pragma once

#include "CoreMinimal.h"
#include "Share/Enums/EDamageReact.h"
#include "Share/Enums/EDamageType.h"
#include "FSDamageInfo.generated.h"


/**
 *  ===============================================
 *  Structure containing information about damage
 *  ===============================================
 */
USTRUCT(BlueprintType)
struct RAIDER_API FSDamageInfo
{
	GENERATED_BODY()

public:
	/** Amount of damage dealt */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Amount;

	/** Type of damage inflicted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageType DamageType;

	/** Reaction type when the target is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageReact DamageReact;

	/** Whether the damage affects invisible entities */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool ShouldDamageInvisible;

	/** Whether the damage interrupts animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool ShouldForceInterrupt;
	
	/** Can the damage be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool CanBeBlocked;

	/** Default constructor */
	FSDamageInfo();

	/** Constructor allowing only damage amount */
	FSDamageInfo(float InAmount);

	/** Constructor allowing full customization */
	FSDamageInfo(float InAmount, EDamageType InDamageType, EDamageReact InDamageReact, bool InShouldDamageInvisible,
	             bool InShouldForceInterrupt, bool InCanBeBlocked);

	/** Converts struct data to a string (for debugging) */
	FString ToString() const;
};

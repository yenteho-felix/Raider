#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Animation/AnimMontage.h"
#include "FComboAttackData.generated.h"

/** Struct to store each combo attack data */
USTRUCT(BlueprintType)
struct FComboAttackData : public FTableRowBase
{
	GENERATED_BODY()

	/** The attack montage to play */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	/** The valid follow-up attacks (X or Y) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FName> NextCombo;  // Maps input (X/Y) to the next attack row name

	/** The time window (0.0 - 1.0) when the combo can be continued */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ComboWindowOpenTime = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ComboWindowCloseTime = 0.95f;	
};

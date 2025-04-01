// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyComboAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDER_API UMyComboAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyComboAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Triggered by gamepad button press */
	void HandleAttackInput(AActor* Target = nullptr);

	/** Reset combo state */
	void ResetCombo();

	/** Check if character is attacking */
	bool IsAttacking() const { return bIsAttacking; }

private:
	/** Play montage at current combo index */
	void PlayComboMontage(int32 ComboIndex);

	/** Montage notify callback */
	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	/** Montage end callback */
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY()
	AActor* CurrentAttackTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Animation")
	TArray<UAnimMontage*> ComboAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Config")
	float PlayerMontagePlayRate = 1.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Config")
	float NPCMontagePlayRate = 1.0f;

	UPROPERTY()
	int32 CurrentComboIndex;

	UPROPERTY()
	bool bIsAttacking;

	UPROPERTY()
	bool bIsNotifyBound;

	UPROPERTY()
	bool bComboHeld;

	UPROPERTY()
	float DefaultWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Config")
	float AttackWalkSpeed;
};

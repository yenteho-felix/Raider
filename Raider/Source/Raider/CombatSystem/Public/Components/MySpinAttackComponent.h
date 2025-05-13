// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystemAPI.h"
#include "Components/ActorComponent.h"
#include "MySpinAttackComponent.generated.h"


class ARaiderCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATSYSTEM_API UMySpinAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMySpinAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Startup animation */
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Animation")
	UAnimMontage* SpinMontage;

	/** Spin rotation speed in degrees/second */
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Config")
	float SpinRotationSpeed = 720.0f;

	/** Max time allowed for spinning in second */
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Config")
	float MaxSpinDuration = 5.0f;
	
	/** Call when spin input is pressed */
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartSpinAttack();

	/** Call when spin input is released */
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopSpinAttack();

private:
	/** Owner character's mesh before spin */
	FRotator OriginalMeshRotation;

	/** Cached reference */
	UPROPERTY()
	ARaiderCharacter* OwnerCharacter;

	/** Handle for spin rotation updates */
	FTimerHandle SpinLoopTimer;

	/** Delay before loop starts */
	FTimerHandle SpinTransitionTimer;

	/** Spin duration timer */
	FTimerHandle SpinDurationTimer;

	/** Whether the spin is active */
	bool bIsSpinning;

	/** Walk speed the system set */
	float DefaultWalkSpeed;

	/** Walk speed while attacking */
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Config")
	float AttackWalkSpeed;
	
	/** Called after SpinStart montage finishes */
	void EnterSpinLoop();

	/** Rotates the mesh during spin loop */
	void UpdateSpin();

	/** Called when an animation notify begins during the spin attack montage */
	UFUNCTION()
	void OnAttackMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};

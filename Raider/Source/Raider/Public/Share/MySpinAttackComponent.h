// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MySpinAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDER_API UMySpinAttackComponent : public UActorComponent
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
	UPROPERTY(EditDefaultsOnly, Category = "SpinAttack|Animation")
	UAnimMontage* SpinMontage;

	/** Spin rotation speed in degrees/second */
	UPROPERTY(EditDefaultsOnly, Category = "SpinAttack|Config")
	float SpinRotationSpeed = 720.0f;
	
	/** Call when spin input is pressed */
	UFUNCTION(BlueprintCallable, Category = "SpinAttack")
	void StartSpinAttack();

	/** Call when spin input is released */
	UFUNCTION(BlueprintCallable, Category = "SpinAttack")
	void StopSpinAttack();

private:
	/** Owner character's mesh before spin */
	FRotator OriginalMeshRotation;

	/** Cached reference */
	UPROPERTY()
	ACharacter* OwnerCharacter;

	/** Handle for spin rotation updates */
	FTimerHandle SpinLoopTimer;

	/** Delay before loop starts */
	FTimerHandle SpinTransitionTimer;

	/** Whether the spin is active */
	bool bIsSpinning;
	
	/** Called after SpinStart montage finishes */
	void EnterSpinLoop();

	/** Rotates the mesh during spin loop */
	void UpdateSpin();
};

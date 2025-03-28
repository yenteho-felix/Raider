// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MySpinAttackComponent.h"

#include "GameFramework/Character.h"


// Sets default values for this component's properties
UMySpinAttackComponent::UMySpinAttackComponent()
	: SpinMontage(nullptr),
	  OwnerCharacter(nullptr),
	  bIsSpinning(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMySpinAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}

void UMySpinAttackComponent::StartSpinAttack()
{
	if (!OwnerCharacter || bIsSpinning)
	{
		return;
	}

	bIsSpinning = true;
	OriginalMeshRotation = OwnerCharacter->GetMesh()->GetRelativeRotation();

	// Play startup montage
	if (SpinMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(SpinMontage);

			// Delay before entering loop
			GetWorld()->GetTimerManager().SetTimer(
				SpinTransitionTimer, this, &UMySpinAttackComponent::EnterSpinLoop, 1.54, false);
		}
	}
	else
	{
		// If no start montage, skip to loop
		EnterSpinLoop();
	}
}

void UMySpinAttackComponent::StopSpinAttack()
{
	if (!OwnerCharacter || !bIsSpinning)
	{
		return;
	}

	bIsSpinning = false;

	// Stop rotation and transition timers
	GetWorld()->GetTimerManager().ClearTimer(SpinLoopTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpinTransitionTimer);

	// Reset mesh rotation
	OwnerCharacter->GetMesh()->SetRelativeRotation(OriginalMeshRotation);

	// Stop spin animation
	if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.1f);
	}
}

void UMySpinAttackComponent::EnterSpinLoop()
{
	// Call UpdateSpin every 0.01 seconds
	GetWorld()->GetTimerManager().SetTimer(
		SpinLoopTimer, this, &UMySpinAttackComponent::UpdateSpin, 0.01f, true);
}

void UMySpinAttackComponent::UpdateSpin()
{
	if (!OwnerCharacter || !bIsSpinning)
	{
		return;
	}

	FRotator NewRotation = OwnerCharacter->GetMesh()->GetRelativeRotation();
	NewRotation.Yaw += SpinRotationSpeed * 0.01f;
	OwnerCharacter->GetMesh()->SetRelativeRotation(NewRotation);
}


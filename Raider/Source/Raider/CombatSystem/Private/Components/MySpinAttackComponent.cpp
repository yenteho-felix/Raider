// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Components/MySpinAttackComponent.h"

#include "RaiderCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/MyCombatComponent.h"
#include "Structs/FSDamageInfo.h"


// Sets default values for this component's properties
UMySpinAttackComponent::UMySpinAttackComponent()
	: SpinMontage(nullptr),
	  OwnerCharacter(nullptr),
	  bIsSpinning(false),
      DefaultWalkSpeed(0),
      AttackWalkSpeed(50)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMySpinAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ARaiderCharacter>(GetOwner());
	DefaultWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
}

void UMySpinAttackComponent::StartSpinAttack()
{
	if (!OwnerCharacter || bIsSpinning)
	{
		return;
	}

	bIsSpinning = true;
	OriginalMeshRotation = OwnerCharacter->GetMesh()->GetRelativeRotation();
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = AttackWalkSpeed;

	// Play startup montage
	if (SpinMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMySpinAttackComponent::OnAttackMontageNotifyBegin);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMySpinAttackComponent::OnAttackMontageNotifyBegin);
			
			AnimInstance->Montage_Play(SpinMontage);

			// Delay before entering loop
			GetWorld()->GetTimerManager().SetTimer(
				SpinTransitionTimer, this, &UMySpinAttackComponent::EnterSpinLoop, 1.54, false);

			// Stop spinning after max duration
			GetWorld()->GetTimerManager().SetTimer(
				SpinDurationTimer, this, &UMySpinAttackComponent::StopSpinAttack, MaxSpinDuration, false);
		}
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
	GetWorld()->GetTimerManager().ClearTimer(SpinDurationTimer);

	// Reset mesh rotation
	OwnerCharacter->GetMesh()->SetRelativeRotation(OriginalMeshRotation);

	// Reset walk speed
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

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

void UMySpinAttackComponent::OnAttackMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName != "Spin" || !OwnerCharacter || !OwnerCharacter->CombatComponent)
	{
		return;
	}
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	
	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	// Perform a multi-hit sphere trace around the player
	const FVector Center = OwnerCharacter->GetActorLocation();
	const float Radius = 150.f;
	
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Center,
		Center,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
	
	// Apply damage to all valid actors in the hit results
	if (bHit && HitResults.Num() > 0)
	{
		FSDamageInfo DamageInfo;
		DamageInfo.Amount = 20;
		DamageInfo.DamageType = EDamageType::Melee;
		DamageInfo.DamageReact = EDamageReact::Hit;

		OwnerCharacter->CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
	}
}


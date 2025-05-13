// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Components//MyComboAttackComponent.h"

#include "RaiderCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/MyCombatComponent.h"


// Sets default values for this component's properties
UMyComboAttackComponent::UMyComboAttackComponent()
	: OwnerCharacter(nullptr),
	  AnimInstance(nullptr),
	  CurrentAttackTarget(nullptr),
	  CurrentComboIndex(0),
	  bIsAttacking(false),
	  bIsNotifyBound(false),
      bComboHeld(false),
	  DefaultWalkSpeed(0),
	  AttackWalkSpeed(50)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMyComboAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

		DefaultWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void UMyComboAttackComponent::HandleAttackInput(AActor* Target)
{
	if (!OwnerCharacter || !AnimInstance || !ComboAttackMontages.IsValidIndex(CurrentComboIndex))
	{
		return;
	}

	bComboHeld = true;
	
	if (!AnimInstance->IsAnyMontagePlaying())
	{
		bIsAttacking = true;
		CurrentAttackTarget = Target;
		PlayComboMontage(CurrentComboIndex);
	}
}

void UMyComboAttackComponent::ResetCombo()
{
	CurrentComboIndex = 0;
	bIsAttacking = false;
	bComboHeld = false;
	CurrentAttackTarget = nullptr;

	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	}
}

void UMyComboAttackComponent::PlayComboMontage(int32 ComboIndex)
{
	if (!AnimInstance || !ComboAttackMontages.IsValidIndex(ComboIndex))
	{
		return;
	}

	UAnimMontage* MontageToPlay = ComboAttackMontages[ComboIndex];
	if (!MontageToPlay)
	{
		return;
	}

	// Slow down character while attacking
	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = AttackWalkSpeed;
	}

	// Bind notify only once
	if (!bIsNotifyBound)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMyComboAttackComponent::OnMontageNotifyBegin);
		bIsNotifyBound = true;
	}

	const float Rate = OwnerCharacter->IsPlayerControlled() ? PlayerMontagePlayRate : NPCMontagePlayRate;
	AnimInstance->Montage_Play(MontageToPlay, Rate);

	// Bind end delegate
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UMyComboAttackComponent::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
}

void UMyComboAttackComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	ARaiderCharacter* RaiderCharacter = Cast<ARaiderCharacter>(OwnerCharacter);
	if (!RaiderCharacter || !RaiderCharacter->CombatComponent)
	{
		return;
	}
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Slash
	if (NotifyName == "Slash")
	{
		const FVector Start = RaiderCharacter->GetActorLocation();
		const FVector End = Start + RaiderCharacter->GetActorForwardVector() * 50.0f;
		const float Radius = 150.0f;
		const float Damage = 10.0f;
		
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
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
			DamageInfo.Amount = Damage;
			DamageInfo.DamageType = EDamageType::Melee;
			DamageInfo.DamageReact = EDamageReact::Hit;
			RaiderCharacter->CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
		}
	}

	// SlashB
	if (NotifyName == "SlashB")
	{
		const FVector Start = RaiderCharacter->GetActorLocation();
		const FVector End = Start + RaiderCharacter->GetActorForwardVector() * 50.0f;
		const float Radius = 150.0f;
		const float Damage = 10.0f;
		
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
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
			DamageInfo.Amount = Damage;
			DamageInfo.DamageType = EDamageType::Melee;
			DamageInfo.DamageReact = EDamageReact::Hit;
			RaiderCharacter->CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
		}
	}

	// SlashC
	if (NotifyName == "SlashC")
	{
		const FVector Start = RaiderCharacter->GetActorLocation();
		const FVector End = Start + RaiderCharacter->GetActorForwardVector() * 300.0f;
		const float Radius = 100.0f;
		const float Damage = 20.0f;
		
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
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
			DamageInfo.Amount = Damage;
			DamageInfo.DamageType = EDamageType::Melee;
			DamageInfo.DamageReact = EDamageReact::Hit;
			RaiderCharacter->CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
		}
	}
}

void UMyComboAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		ResetCombo();
		return;
	}
	
	if (bComboHeld)
	{
		CurrentComboIndex = (CurrentComboIndex + 1) % ComboAttackMontages.Num();
		PlayComboMontage(CurrentComboIndex);
	}
	else
	{
		ResetCombo();
	}
}



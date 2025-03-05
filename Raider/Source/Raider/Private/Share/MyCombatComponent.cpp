// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MyCombatComponent.h"

#include "DelayAction.h"
#include "TimerManager.h"
#include "Share/MyCombatInterface.h"
#include "Share/Struct/FSDamageInfo.h"
#include "Weapon/WeaponBase.h"
#include "WorldPartition/HLOD/DestructibleHLODComponent.h"


// Sets default values for this component's properties
UMyCombatComponent::UMyCombatComponent()
	: IsWeaponEquipped(false),
	  AttackRadius(150),
	  DefendRadius(250),
	  bIsInvincible(false),
	  bIsBlocking(false),
	  bIsInterruptible(true)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyCombatComponent::EquipWeapon()
{
	if (EquipMontage && WeaponActorClass)
	{
		WeaponActorObj = GetWorld()->SpawnActor<AWeaponBase>(WeaponActorClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (const AActor* Owner = GetOwner())
		{
			if (USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
			{
				AttachWeaponToSocket(WeaponActorObj);
				PlayEquipMontage(EquipMontage);
				IsWeaponEquipped = true;
			}
		}
	}
}

void UMyCombatComponent::UnEquipWeapon()
{
	if (UnEquipMontage && WeaponActorObj)
	{
		if (const AActor* Owner = GetOwner())
		{
			PlayUnEquipMontage(UnEquipMontage);
			GetWorld()->DestroyActor(WeaponActorObj);
			IsWeaponEquipped = false;
		}
	}
}

void UMyCombatComponent::AttachWeaponToSocket(AActor* WeaponActor) const
{
	if (const AActor* Owner = GetOwner())
	{
		USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshComponent->DoesSocketExist(WeaponSocketName))
		{
			if (WeaponActor)
			{
				WeaponActor->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WeaponActor %s does not exists on %s"), *WeaponActor->GetName(), *Owner->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Socket %s does not exists on %s"), *WeaponSocketName.ToString(), *Owner->GetName());
		}
	}
}

void UMyCombatComponent::PlayEquipMontage(UAnimMontage* AnimMontage)
{
	if (!AnimMontage)
	{
		return;
	}

	if (const AActor* Owner = GetOwner())
	{
		if (const USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AnimMontage);

				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnEquipMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
			}
		}
	}
}

void UMyCombatComponent::PlayUnEquipMontage(UAnimMontage* AnimMontage)
{
	if (!AnimMontage)
	{
		return;
	}

	if (const AActor* Owner = GetOwner())
	{
		if (const USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AnimMontage);

				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnUnEquipMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
			}
		}
	}
}

TArray<AActor*> UMyCombatComponent::DamageAllNoneTeamMembers(const TArray<FHitResult>& HitResult, const FSDamageInfo& DamageInfo)
{
	// Clear DamagedActors
	DamagedActors.Empty();
	
	// Get team number of the owner
	int32 MyTeam = 0;
	AActor* OwnerActor = GetOwner();
	if (OwnerActor && OwnerActor->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
	{
		MyTeam = IMyCombatInterface::Execute_GetTeamNumber(GetOwner());
	}

	// Iterate through all hit actors
	for (const FHitResult& Hit : HitResult)
	{
		AActor* HitActor = Hit.GetActor();

		// Skip if invalid or already take damaged
		if (!HitActor || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		// Get team number of the hit actor
		int32 OtherTeam = 0;
		if (HitActor->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
		{
			OtherTeam = IMyCombatInterface::Execute_GetTeamNumber(HitActor);
		}

		// Damage only if the hit actor is on a different team
		if (OtherTeam != MyTeam)
		{
			IMyCombatInterface::Execute_TakeDamage(HitActor, DamageInfo);
			DamagedActors.Add(HitActor);
		}
	}

	return DamagedActors;
}

AActor* UMyCombatComponent::DamageFirstNoneTeamMembers(const TArray<FHitResult>& HitResult, const FSDamageInfo& DamageInfo)
{
	// Clear DamagedActors
	DamagedActors.Empty();
	
	// Get team number of the owner
	int32 MyTeam = 0;
	AActor* OwnerActor = GetOwner();
	if (OwnerActor && OwnerActor->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
	{
		MyTeam = IMyCombatInterface::Execute_GetTeamNumber(GetOwner());
	}

	// Iterate through all hit actors
	for (const FHitResult& Hit : HitResult)
	{
		AActor* HitActor = Hit.GetActor();

		// Skip if invalid or already take damaged
		if (!HitActor || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		// Get team number of the hit actor
		int32 OtherTeam = 0;
		if (HitActor->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
		{
			OtherTeam = IMyCombatInterface::Execute_GetTeamNumber(HitActor);
		}

		// Damage only if the hit actor is on a different team
		if (OtherTeam != MyTeam)
		{
			IMyCombatInterface::Execute_TakeDamage(HitActor, DamageInfo);
			return HitActor;
		}
	}
	
	return nullptr;
}

void UMyCombatComponent::OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnEquipWeaponEnd.Broadcast();
}

void UMyCombatComponent::OnUnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnUnEquipWeaponEnd.Broadcast();
}

void UMyCombatComponent::GetCombatRange(float& OutAttackRadius, float& OutDefendRadius) const
{
	OutAttackRadius = AttackRadius;
	OutDefendRadius = DefendRadius;
}

void UMyCombatComponent::Attack(AActor* AttackTarget)
{
	AActor* Owner = GetOwner();

	if (!Owner) return;

	PlayAttackMontage(AttackMontage);

	CurrentAttackTarget = AttackTarget;

	// Disable this since we move the token request to AI behavior tree
	// // Check if the attack target implements the interface
	// if (AttackTarget->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
	// {
	// 	if (IMyCombatInterface::Execute_RequestAttackToken(AttackTarget, Owner, 1))
	// 	{
	// 		CurrentAttackTarget = AttackTarget;
	// 		PlayAttackMontage(AttackMontage);
	// 	}
	// 	else
	// 	{
	// 		// Delay execution by one tick to allow event listeners to bind
	// 		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UMyCombatComponent::TriggerOnAttackEnd);
	// 	}	
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("%s is does not implement the combat interface!"), *AttackTarget->GetName());
	// }
}

void UMyCombatComponent::PlayAttackMontage(UAnimMontage* AnimMontage)
{
	if (!AnimMontage)
	{
		return;
	}

	if (const AActor* Owner = GetOwner())
	{
		if (const USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				// Ensure binding happens only once
				if (!bIsNotifyBound)
				{
					// Bind to montage notify
					AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMyCombatComponent::OnAttackMontageNotifyBegin);
					bIsNotifyBound = true;
				}
				
				AnimInstance->Montage_Play(AnimMontage);
				
				// Bind to montage end event
				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnAttackMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
			}
		}
	}
}

void UMyCombatComponent::OnAttackMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	OnAttackMontageNotify.Broadcast(NotifyName);
}

void UMyCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	TriggerOnAttackEnd();
}

void UMyCombatComponent::TriggerOnAttackEnd()
{
	CurrentAttackTarget = nullptr;
	OnAttackEnd.Broadcast();
}

bool UMyCombatComponent::ShouldProcessDamage(const FSDamageInfo& DamageInfo) const
{
	// Determine the type of damage handling
	if (bIsBlocking && DamageInfo.CanBeBlocked)
	{
		// Block damage
		OnDamageBlocked.Broadcast();
		return false;
	}

	if (bIsInvincible && !DamageInfo.ShouldDamageInvisible)
	{
		// No damage taken due to invincibility
		return false;
	}

	// Handle reaction to damage
	if (bIsInterruptible || DamageInfo.ShouldForceInterrupt)
	{
		OnDamageReact.Broadcast(DamageInfo.DamageReact);
	}
	return true;
}

void UMyCombatComponent::TakeHit()
{
	if (TakeHitMontage)
	{
		if (const AActor* Owner = GetOwner())
		{
			PlayTakeHitMontage(TakeHitMontage);
		}
	}
}

void UMyCombatComponent::PlayTakeHitMontage(UAnimMontage* AnimMontage)
{
	if (!AnimMontage)
	{
		return;
	}

	if (const AActor* Owner = GetOwner())
	{
		if (const USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AnimMontage);

				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnTakeHitMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
			}
		}
	}
}

void UMyCombatComponent::OnTakeHitMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnTakeHitEnd.Broadcast();
}


// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MyCombatComponent.h"

#include "DelayAction.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Share/MyCombatInterface.h"
#include "Share/Struct/FSDamageInfo.h"
#include "Weapon/WeaponBase.h"
#include "WorldPartition/HLOD/DestructibleHLODComponent.h"


// Sets default values for this component's properties
UMyCombatComponent::UMyCombatComponent()
	: IsWeaponEquipped(false),
	  CurrentAttackTarget(nullptr),
	  PlayerMontagePlayRate(1.3f),
      NPCMontagePlayRate(1.0f),
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
				AttachWeaponToSocket(WeaponActorObj, WeaponSocketName);
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

void UMyCombatComponent::EquipShield()
{
	if (ShieldActorClass)
	{
		ShieldActorObj = GetWorld()->SpawnActor<AWeaponBase>(ShieldActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (const AActor* Owner = GetOwner())
		{
			AttachWeaponToSocket(ShieldActorObj, ShieldSocketName);
		}
	}
}

void UMyCombatComponent::AttachWeaponToSocket(AActor* WeaponActor, const FName SocketName) const
{
	if (const AActor* Owner = GetOwner())
	{
		USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshComponent->DoesSocketExist(SocketName))
		{
			if (WeaponActor)
			{
				WeaponActor->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WeaponActor %s does not exists on %s"), *WeaponActor->GetName(), *Owner->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Socket %s does not exists on %s"), *SocketName.ToString(), *Owner->GetName());
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

	// Iterate through all hit actors
	for (const FHitResult& Hit : HitResult)
	{
		AActor* HitActor = Hit.GetActor();

		// Skip if invalid or already take damaged
		if (!HitActor || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		// Damage only if the hit actor is on a different team
		if (!IsOnSameTeam(GetOwner(), HitActor))
		{
			IMyCombatInterface::Execute_TakeDamage(HitActor, GetOwner(), DamageInfo);
			DamagedActors.Add(HitActor);
		}
	}

	return DamagedActors;
}

AActor* UMyCombatComponent::DamageFirstNoneTeamMembers(const TArray<FHitResult>& HitResult, const FSDamageInfo& DamageInfo)
{
	// Clear DamagedActors
	DamagedActors.Empty();

	// Iterate through all hit actors
	for (const FHitResult& Hit : HitResult)
	{
		AActor* HitActor = Hit.GetActor();

		// Skip if invalid or already take damaged
		if (!HitActor || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		// Damage only if the hit actor is on a different team
		if (!IsOnSameTeam(GetOwner(), HitActor))
		{
			IMyCombatInterface::Execute_TakeDamage(HitActor, GetOwner(), DamageInfo);
			return HitActor;
		}
	}
	
	return nullptr;
}

bool UMyCombatComponent::IsOnSameTeam(AActor* OwnerActor, AActor* OtherActor) const
{
	if (!OwnerActor || !OwnerActor->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
	{
		return false;
	}
	
	if (!OtherActor || !OtherActor->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
	{
		return false;
	}

	const int32 MyTeam = IMyCombatInterface::Execute_GetTeamNumber(GetOwner());
	const int32 OtherTeam = IMyCombatInterface::Execute_GetTeamNumber(OtherActor);

	return (MyTeam == OtherTeam);
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
	if (!GetOwner() || !AttackMontage)
	{
		return;
	}

	PlayAttackMontage(AttackMontage);
	
	CurrentAttackTarget = AttackTarget;
}

void UMyCombatComponent::PlayAttackMontage(UAnimMontage* AnimMontage)
{
	if (!AnimMontage)
	{
		return;
	}

	if (AActor* Owner = GetOwner())
	{
		if (USkeletalMeshComponent* MeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			UAnimInstance* AttackAnimInstance = MeshComponent->GetAnimInstance();

			// Bind to montage notify
			AttackAnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMyCombatComponent::OnAttackMontageNotifyBegin);
			AttackAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMyCombatComponent::OnAttackMontageNotifyBegin);

			// Adjust play rate based on owner type
			if (const ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner()))
			{
				const float MontagePlayRate = CharacterOwner->IsPlayerControlled() ? PlayerMontagePlayRate : NPCMontagePlayRate;
				AttackAnimInstance->Montage_Play(AnimMontage, MontagePlayRate);
			}
	
			// Bind to montage end event
			FOnMontageEnded MontageEndDelegate;
			MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnAttackMontageEnded);
			AttackAnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
		}			
	}
}

void UMyCombatComponent::OnAttackMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	OnAttackMontageNotify.Broadcast(NotifyName);
}

void UMyCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentAttackTarget = nullptr;
	TriggerOnAttackEnd();
}

void UMyCombatComponent::TriggerOnAttackEnd()
{
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
		PlayTakeHitMontage(TakeHitMontage);
	}
}

void UMyCombatComponent::Block()
{
	if (BlockMontage)
	{
		PlayBlockingMontage(BlockMontage);
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

void UMyCombatComponent::PlayBlockingMontage(UAnimMontage* AnimMontage)
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
				// Bind to montage notify
				AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMyCombatComponent::OnBlockingMontageNotifyBegin);
				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMyCombatComponent::OnBlockingMontageNotifyBegin);

				// Play montage
				AnimInstance->Montage_Play(AnimMontage);

				// Montage end delegate
				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnBlockingMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
			}
		}
	}
}

void UMyCombatComponent::OnTakeHitMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnTakeHitEnd.Broadcast();
}

void UMyCombatComponent::OnBlockingMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "BlockStart")
	{
		bIsBlocking = true;
	}
}

void UMyCombatComponent::OnBlockingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsBlocking = false;
}


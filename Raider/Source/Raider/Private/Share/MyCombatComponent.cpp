﻿// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "Share/MyCombatComponent.h"

#include "Weapon/WeaponBase.h"


// Sets default values for this component's properties
UMyCombatComponent::UMyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default value
	AttackRadius = 150.0f;
	DefendRadius = 250.0f;
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

void UMyCombatComponent::OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnEquipWeaponEnd.Broadcast();
}

void UMyCombatComponent::OnUnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnUnEquipWeaponEnd.Broadcast();
}

// void UMyCombatComponent::TriggerOnEquipWeaponEnd() const
// {
// 	OnEquipWeaponEnd.Broadcast();
// }

// void UMyCombatComponent::TriggerOnUnEquipWeaponEnd() const
// {
// 	OnUnEquipWeaponEnd.Broadcast();
// }

void UMyCombatComponent::GetCombatRange(float& OutAttackRadius, float& OutDefendRadius) const
{
	OutAttackRadius = AttackRadius;
	OutDefendRadius = DefendRadius;
}

void UMyCombatComponent::Attack()
{
	if (const AActor* Owner = GetOwner())
	{
		PlayAttackMontage(AttackMontage);
	}
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
				AnimInstance->Montage_Play(AnimMontage);

				FOnMontageEnded MontageEndDelegate;
				MontageEndDelegate.BindUObject(this, &UMyCombatComponent::OnAttackMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimMontage);
			}
		}
	}
}

void UMyCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	OnAttackEnd.Broadcast();
}

// void UMyCombatComponent::TriggerOnAttackEnd() const
// {
// 	OnAttackEnd.Broadcast();
// }


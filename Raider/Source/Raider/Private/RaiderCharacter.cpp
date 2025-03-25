// Copyright Epic Games, Inc. All Rights Reserved.

#include "RaiderCharacter.h"

#include "RaiderPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Share/MyCombatComponent.h"
#include "Share/MyHealthComponent.h"

ARaiderCharacter::ARaiderCharacter()
	: TeamNumber(255),
	  bIsAttacking(false),
      AttackTokenCount(1)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->SetRelativeLocation(FVector(-500.0f, 0.0f, 2000.0f));
	TopDownCameraComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Health Component
	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>("HealthComponent");
	HealthComponent->AttackTokenCount = AttackTokenCount;

	// Combat Component
	CombatComponent = CreateDefaultSubobject<UMyCombatComponent>("CombatComponent");
	
}

void ARaiderCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

bool ARaiderCharacter::RequestAttackToken_Implementation(AActor* RequestingAttacker, const int Amount)
{
	if (!HealthComponent)
	{
		return true;
	}
	return HealthComponent->RequestAttackToken(RequestingAttacker, Amount);
}

void ARaiderCharacter::ReturnAttackToken_Implementation(AActor* RequestingAttacker, const int Amount)
{
	if (HealthComponent)
	{
		HealthComponent->ReturnAttackToken(RequestingAttacker, Amount);
	}
}

int32 ARaiderCharacter::GetTeamNumber_Implementation()
{
	return TeamNumber;
}

bool ARaiderCharacter::TakeDamage_Implementation(AActor* Attacker, const FSDamageInfo& DamageInfo)
{
	if (!HealthComponent || !CombatComponent)
	{
		return false;
	}
	
	// Check if combat logic allows damage to be applied
	if (CombatComponent->ShouldProcessDamage(DamageInfo))
	{
		HealthComponent->TakeDamage(DamageInfo.Amount);
		CombatComponent->TakeHit();
		return true;
	}

	return false;
}

float ARaiderCharacter::GetMaxHealth_Implementation()
{
	if (!HealthComponent)
	{
		return 0;
	}

	return HealthComponent->MaxHealth;
}

float ARaiderCharacter::GetCurrentHealth_Implementation()
{
	if (!HealthComponent)
	{
		return 0;
	}

	return HealthComponent->Health;
}

bool ARaiderCharacter::IsDead_Implementation()
{
	if (!HealthComponent)
	{
		return false;
	}
	return !HealthComponent->IsAlive();
}

void ARaiderCharacter::OnAttackMontageNotifyHandler_Implementation(FName NotifyName)
{
	//UE_LOG(LogTemp, Warning, TEXT("Called C++ Default Attack Montage Notify"));


    // Perform a multi-hit sphere trace in front of character
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	if (NotifyName == "Slash")
	{
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + (GetActorForwardVector() * 150.0f),
			50.0f,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true
		);
		
		// Apply damage to all valid actors in the hit results
		FSDamageInfo DamageInfo;
		DamageInfo.Amount = 10;
		DamageInfo.DamageType = EDamageType::Melee;
		DamageInfo.DamageReact = EDamageReact::Hit;
		if (bHit && HitResults.Num() > 0)
		{
			CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
		}
	}

	if (NotifyName == "SlashB")
	{
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + (GetActorForwardVector() * 50.0f),
			150.0f,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true
		);
		
		// Apply damage to all valid actors in the hit results
		FSDamageInfo DamageInfo;
		DamageInfo.Amount = 10;
		DamageInfo.DamageType = EDamageType::Melee;
		DamageInfo.DamageReact = EDamageReact::Hit;
		if (bHit && HitResults.Num() > 0)
		{
			CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
		}
	}

	if (NotifyName == "SlashC")
	{
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + (GetActorForwardVector() * 300.0f),
			100.0f,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResults,
			true
		);
		
		// Apply damage to all valid actors in the hit results
		FSDamageInfo DamageInfo;
		DamageInfo.Amount = 20;
		DamageInfo.DamageType = EDamageType::Melee;
		DamageInfo.DamageReact = EDamageReact::Hit;
		if (bHit && HitResults.Num() > 0)
		{
			CombatComponent->DamageAllNoneTeamMembers(HitResults, DamageInfo);
		}
	}

	
}

void ARaiderCharacter::OnDeathHandler_Implementation()
{
	//Play death animation
	if (HealthComponent)
	{
		if (HealthComponent->DeathMontage)
		{
			HealthComponent->PlayDeathMontage(HealthComponent->DeathMontage);
		}
		else
		{
			HealthComponent->PlayDeathRagDoll();
		}
	}

	// Stop movement & collision
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable Input and set input mode to UI-only
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void ARaiderCharacter::OnAttackEndHandler()
{
	bIsAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void ARaiderCharacter::OnDamageBlockedHandler()
{
	const float Force = 300.0f;
	const FVector Backward = -GetActorForwardVector();
	LaunchCharacter(Backward * Force, true, false);
}

void ARaiderCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CombatComponent)
	{
		// Prevent multiple bindings by unbinding first
		CombatComponent->OnAttackMontageNotify.RemoveDynamic(this, &ARaiderCharacter::OnAttackMontageNotifyHandler);
		CombatComponent->OnAttackEnd.RemoveDynamic(this, &ARaiderCharacter::OnAttackEndHandler);
		CombatComponent->OnDamageBlocked.RemoveDynamic(this, &ARaiderCharacter::OnDamageBlockedHandler);

		// Bind the delegates
		CombatComponent->OnAttackMontageNotify.AddDynamic(this, &ARaiderCharacter::OnAttackMontageNotifyHandler);
		CombatComponent->OnAttackEnd.AddDynamic(this, &ARaiderCharacter::OnAttackEndHandler);
		CombatComponent->OnDamageBlocked.AddDynamic(this, &ARaiderCharacter::OnDamageBlockedHandler);
	}

	if (HealthComponent)
	{
		// Prevent multiple bindings by unbinding first
		HealthComponent->OnDeath.RemoveDynamic(this, &ARaiderCharacter::OnDeathHandler);
		
		// Bind the delegates
		HealthComponent->OnDeath.AddDynamic(this, &ARaiderCharacter::OnDeathHandler);
	}
	
	// Add Player UI
	if (PlayerUIClass)
	{
		AddWidgetToViewPort(PlayerUIClass, false);
	}

	EquipWeapon_Implementation();

	// Store the default walk speed
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void ARaiderCharacter::AddWidgetToViewPort(const TSubclassOf<UUserWidget>& InWidgetClass, bool bShowMouseCursor) const
{
	if (InWidgetClass)
	{
		if (UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InWidgetClass))
		{
			WidgetInstance->AddToViewport();
		}
		
		if (APlayerController* MyPlayerController = Cast<APlayerController>(GetController()))
		{
			MyPlayerController->SetShowMouseCursor(bShowMouseCursor);
		}
	}
}

void ARaiderCharacter::EquipWeapon_Implementation()
{
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->EquipWeapon();
	CombatComponent->EquipShield();
}

void ARaiderCharacter::Block_Implementation()
{
	if (!CombatComponent)
	{
		return;
	}

	// Skip block command if player is blocking already
	if (!CombatComponent->bIsBlocking)
	{
		CombatComponent->Block();
	}
}

void ARaiderCharacter::LightAttack() 
{
	if (!CombatComponent)
	{
		return;
	}

	// If the player presses attack again during the combo window, queue the next attack
	if (bIsAttacking && CombatComponent->IsComboWindowOpen()) 
	{
		CombatComponent->Attack(nullptr);
		return;
	}

	// Start the first attack
	bIsAttacking = true;
	GetCharacterMovement()->MaxWalkSpeed = 50.0f; // Slowing movement during attack
	CombatComponent->Attack(nullptr);
}

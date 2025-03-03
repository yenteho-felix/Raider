// Copyright © 2025 Felix Ho. All Rights Reserved.


#include "NPC/NPCAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/NPCCharacterBase.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"


ANPCAIController::ANPCAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create blackboard component
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	
	// AI perception component
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");

	// Setup sight config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->SetMaxAge(2.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Setup hearing config
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("HearingConfig");
	HearingConfig->HearingRange = 1500.0f;
	HearingConfig->SetMaxAge(2.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Setup damage config
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageConfig");
	DamageConfig->SetMaxAge(2.0f);

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ANPCAIController::OnPerceptionUpdated);
}

void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get owner character
	OwnerCharacter = Cast<ANPCCharacterBase>(GetPawn());
	if (!OwnerCharacter)
	{
		return;
	}
	
	// Run behavior tree
	if (OwnerCharacter && OwnerCharacter->BehaviorTreeAsset)
	{
		UseBlackboard(OwnerCharacter->BehaviorTreeAsset->BlackboardAsset, BlackboardComponent);
		RunBehaviorTree(OwnerCharacter->BehaviorTreeAsset);
		SetStateAsPassive();
		SetCombatRange();
	}
}

void ANPCAIController::SetStateAsPassive() const
{
	BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::Passive));
}

void ANPCAIController::SetStateAsFrozen() const
{
	BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::Frozen));
}

void ANPCAIController::SetStateAsAttacking(AActor* TargetActor)
{
	BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::Attacking));
	BlackboardComponent->SetValueAsObject("AttackTarget", TargetActor);
	AttackTarget = TargetActor;
}

void ANPCAIController::SetStateAsInvestigating(const FVector Location) const
{
	BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::Investigating));
	BlackboardComponent->SetValueAsVector("Location", Location);
}

void ANPCAIController::SetStateAsDead() const
{
	BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::Dead));
}

EAIState ANPCAIController::GetCurrentState() const
{
	return static_cast<EAIState>(BlackboardComponent->GetValueAsEnum("AIState"));
}

void ANPCAIController::SetCombatRange() const
{
	if (!OwnerCharacter)
	{
		return;
	}

	// Check if OwnerCharacter implements INPCCombatInterface
	IMyCombatInterface* CombatInterface = Cast<IMyCombatInterface>(OwnerCharacter);
	if (!CombatInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is does not implement the combat interface!"), *OwnerCharacter->GetName());
		return;
	}
	
	float DefendRadius;
	float AttackRadius;
	CombatInterface->Execute_GetCombatRange(OwnerCharacter, AttackRadius, DefendRadius);
	
	BlackboardComponent->SetValueAsFloat("AttackRadius", AttackRadius);
	BlackboardComponent->SetValueAsFloat("DefendRadius", DefendRadius);

}

void ANPCAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor)
		{
			FAIStimulus Stimulus;

			if (CanSenseActor(Actor, EAISense::Sight,Stimulus))
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s AI Sees Actor: %s"), *OwnerCharacter->GetName(), *Actor->GetName());
				HandleSenseSight(Actor);
			}

			if (CanSenseActor(Actor, EAISense::Hearing, Stimulus))
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s AI Heard Actor: %s"), *OwnerCharacter->GetName(), *Actor->GetName());
				HandleSenseSound(Stimulus.StimulusLocation);
			}

			if (CanSenseActor(Actor, EAISense::Damage, Stimulus))
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s AI Detected Damage from Actor: %s"), *OwnerCharacter->GetName(), *Actor->GetName());
				HandleSenseDamage(Actor);
			}
		}
	}
}

bool ANPCAIController::CanSenseActor(AActor* Actor, const EAISense SenseType, FAIStimulus& OutStimulus) const
{
	if (!Actor || !AIPerceptionComponent)
	{
		return false;
	}

	FActorPerceptionBlueprintInfo PerceptionInfo;
	AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

	for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
	{
		if ((SenseType == EAISense::Sight && Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()) ||
			(SenseType == EAISense::Hearing && Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>()) ||
			(SenseType == EAISense::Damage && Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>()))
		{
			OutStimulus = Stimulus;
			return Stimulus.WasSuccessfullySensed();
		}
	}
	return false;
}

void ANPCAIController::HandleSenseSight(AActor* TargetActor)
{
	// if (TargetActor != UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	// 	return;
	
	EAIState CurrentState = GetCurrentState();
	if (CurrentState == EAIState::Passive || CurrentState == EAIState::Investigating)
	{
		SetStateAsAttacking(TargetActor);
	}
}

void ANPCAIController::HandleSenseSound(const FVector& Location) const
{
	EAIState CurrentState = GetCurrentState();
	if (CurrentState == EAIState::Passive || CurrentState == EAIState::Investigating)
	{
		SetStateAsInvestigating(Location);
	}
}

void ANPCAIController::HandleSenseDamage(AActor* Actor)
{
	EAIState CurrentState = GetCurrentState();
	if (CurrentState == EAIState::Passive || CurrentState == EAIState::Investigating)
	{
		SetStateAsAttacking(Actor);
	}
}
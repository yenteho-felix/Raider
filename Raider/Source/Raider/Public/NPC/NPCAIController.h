// Copyright © 2025 Felix Ho. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/EAISense.h"
#include "Enums/EAIState.h"
#include "NPCAIController.generated.h"

class ANPCCharacterBase;
struct FAIStimulus;
class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
class UAIPerceptionComponent;

/**
 *  Initializes perception components for Sight, Hearing, and Damage.
 *  Sets up blackboard and registers perception updates.
 */
UCLASS()
class RAIDER_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCAIController();

protected:
	virtual void BeginPlay() override;

/**
 *  Behavior Tree and Blackboard
 */
public:
	/** Sets AI attack target which can be accessible by EQS query */
	UPROPERTY(BlueprintReadOnly, Category = "NPC|AIState")
	TObjectPtr<AActor> AttackTarget;
	
	/** Sets AI state to Passive */
	UFUNCTION(BlueprintCallable, Category = "NPC|AIState")
	void SetStateAsPassive() const;

	/** Sets AI state to Attacking and assigns a target */
	UFUNCTION(BlueprintCallable, Category = "NPC|AIState")
	void SetStateAsAttacking(AActor* TargetActor);

	/** Sets AI state to Investigating and updates the target location */
	UFUNCTION(BlueprintCallable, Category = "NPC|AIState")
	void SetStateAsInvestigating(const FVector Location) const;
	
protected:
	/** Blackboard component to store blackboard instance we used */
	UPROPERTY(EditAnywhere, Category = "NPC|BehaviorTree")
	UBlackboardComponent* BlackboardComponent;

	/** Gets the current AI state from the blackboard */
	EAIState GetCurrentState() const;

private:
	/** The character who owns this AI controller */
	UPROPERTY()
	ANPCCharacterBase* OwnerCharacter;
	
/**
 *  AI Perception System
 */
protected:
	/** AI perception component that manage the AI's ability to sense the environment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AI Perception")
	UAIPerceptionComponent* AIPerceptionComponent;

	/** Used to detect visual stimuli */
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	/** Used to detect sound stimuli */
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	/** Used to detect when the AI takes damage */
	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;

	/** Handles updates form the AI perception system. Checks which actors were perceived and reacts accordingly */
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	/** Checks if the AI can sense a given actor using a specified sense */
	UFUNCTION()
	bool CanSenseActor(AActor* Actor,const EAISense SenseType, FAIStimulus& OutStimulus) const;

	/** Handles AI response to sight perception */
	UFUNCTION()
	void HandleSenseSight(AActor* TargetActor);

	/** Handles AI response to sound perception */
	UFUNCTION()
	void HandleSenseSound(const FVector& Location) const;

	/** Handles AI response to detecting damage */
	UFUNCTION()
	void HandleSenseDamage(AActor* Actor);
};

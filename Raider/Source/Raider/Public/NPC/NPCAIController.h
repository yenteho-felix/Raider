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
	UFUNCTION(BlueprintCallable, Category = "NPC|BehaviorTree")
	void SetStateAsPassive() const;

	UFUNCTION(BlueprintCallable, Category = "NPC|BehaviorTree")
	void SetStateAsAttacking(AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable, Category = "NPC|BehaviorTree")
	void SetStateAsInvestigating(const FVector Location) const;
	
protected:
	UPROPERTY(EditAnywhere, Category = "NPC|BehaviorTree")
	UBlackboardComponent* BlackboardComponent;

	EAIState GetCurrentState() const;

private:
	ANPCCharacterBase* OwnerCharacter;
	
/**
 *  AI Perception System
 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|AI Perception")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	UFUNCTION()
	bool CanSenseActor(AActor* Actor,const EAISense SenseType, FAIStimulus& OutStimulus) const;

	void HandleSenseSight(AActor* TargetActor) const;

	void HandleSenseSound(const FVector& Location) const;

	void HandleSenseDamage(AActor* Actor) const;
};

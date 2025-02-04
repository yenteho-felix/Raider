// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaiderGameMode.generated.h"

/**
 *	GameMode for project Raider
 */
UCLASS(minimalapi)
class ARaiderGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARaiderGameMode();

protected:
	virtual void BeginPlay() override;
	
/**
 *	NPC related
 */
public:
	/* Define enemy class */
	UPROPERTY(EditAnywhere, Category = "Default|NPC")
	TSubclassOf<AActor> EnemyClass;

	/* Define NPC spawn point class */
	UPROPERTY(EditAnywhere, Category = "Default|NPC")
	TSubclassOf<AActor> NPCSpawnPointClass;
	
	/* Spawn new wave of enemies */
	UFUNCTION()
	void StartEnemySpawn(const TSubclassOf<AActor>& InEnemyClass, const TSubclassOf<AActor>& InSpawnPointClass) const;
	
protected:
	/* Search NPC spawn points in the level to spawn NPC */
	UFUNCTION()
	FTransform GetRandomSpawnPoints(const TSubclassOf<AActor>& InSpawnPointClass) const;

	/* Spawn enemy by given enemy class */
	void SpawnEnemy(const TSubclassOf<AActor>& InEnemyClass, const FTransform& InSpawnPoint) const;
};




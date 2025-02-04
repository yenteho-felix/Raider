// Copyright Epic Games, Inc. All Rights Reserved.

#include "RaiderGameMode.h"
#include "RaiderPlayerController.h"
#include "RaiderCharacter.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ARaiderGameMode::ARaiderGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARaiderPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Raider/Blueprints/Characters/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Raider/Blueprints/BP_PlayerController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ARaiderGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartEnemySpawn(EnemyClass, NPCSpawnPointClass);
}

void ARaiderGameMode::StartEnemySpawn(const TSubclassOf<AActor>& InEnemyClass, const TSubclassOf<AActor>& InSpawnPointClass) const
{
	if (!InEnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InEnemyClass is NULL"));
		return;
	}
	
	if (!InSpawnPointClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InSpawnPointClass is NULL"));
		return;
	}
	
	// Spawn enemy at each NPC spawn points in the level
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), InSpawnPointClass, SpawnPoints);

	if (SpawnPoints.Num() > 0)
	{
		for (int i = 0; i < SpawnPoints.Num(); i++)
		{
			FTransform SpawnPoint = SpawnPoints[i]->GetTransform();
			SpawnEnemy(InEnemyClass, SpawnPoint);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No NPC Spawn Points Found"));
	}
}

FTransform ARaiderGameMode::GetRandomSpawnPoints(const TSubclassOf<AActor>& InSpawnPointClass) const
{
	if (!InSpawnPointClass)
	{
		return FTransform();
	}

	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), InSpawnPointClass, SpawnPoints);

	if (SpawnPoints.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		return SpawnPoints[RandomIndex]->GetTransform();
	}
	
	return FTransform();
}

void ARaiderGameMode::SpawnEnemy(const TSubclassOf<AActor>& InEnemyClass, const FTransform& InSpawnPoint) const
{
	if (InEnemyClass)
	{
		GetWorld()->SpawnActor<AActor>(InEnemyClass, InSpawnPoint);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InEnemyClass is NULL"));
	}
}

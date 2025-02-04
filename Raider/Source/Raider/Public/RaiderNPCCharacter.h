// Game Development Exercise by Felix Ho

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RaiderNPCCharacter.generated.h"

UCLASS()
class RAIDER_API ARaiderNPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARaiderNPCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

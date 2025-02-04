// Game Development Exercise by Felix Ho


#include "RaiderNPCCharacter.h"

// Sets default values
ARaiderNPCCharacter::ARaiderNPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARaiderNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARaiderNPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARaiderNPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


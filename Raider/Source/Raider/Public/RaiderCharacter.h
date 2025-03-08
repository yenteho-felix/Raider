// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Share/MyCombatInterface.h"
#include "RaiderCharacter.generated.h"

class UMyCombatComponent;
class UMyHealthComponent;

UCLASS(Blueprintable)
class ARaiderCharacter : public ACharacter, public IMyCombatInterface
{
	GENERATED_BODY()

public:
	ARaiderCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	virtual void BeginPlay() override;
	
/**
 *	---------------------------------------------
 *	General
 *	---------------------------------------------
 */
private:
	/** Default team number */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Team", meta = (AllowPrivateAccess = "true"))
	int32 TeamNumber;

/**
 *	---------------------------------------------
 *	Widget
 *	---------------------------------------------
 */
public:
	/** Variable to store game play widget */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|Widget")
	TSubclassOf<UUserWidget> PlayerUIClass;
	
protected:
	/** Add give widget class to the viewport */
	void AddWidgetToViewPort(const TSubclassOf<UUserWidget>& InWidgetClass, bool bShowMouseCursor) const;
	
/**
 *  ----------------------------------------------
 *	Combat Related
 *  ----------------------------------------------
 */
public:
	/** Component handling combat-related functionalities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Combat")
	UMyCombatComponent* CombatComponent;

	UFUNCTION(Category = "Player")
	void LightAttack();
	
protected:
	/** NPCCombatInterface, equip weapon function */
	UFUNCTION(Category = "Player")
	virtual void EquipWeapon_Implementation() override;
	
/**
 *  ----------------------------------------------
 *	Health Related
 *  ----------------------------------------------
 */
public:
	/** Component handling health-related functionalities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Health")
	UMyHealthComponent* HealthComponent;
	
	/** The maximum number of enemies who can attack the player simultaneously */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Health")
	int32 AttackTokenCount;

	/** NPCCombatInterface, request token */
	UFUNCTION(Category = "Player")
	virtual bool RequestAttackToken_Implementation(AActor* RequestingAttacker, int Amount) override;

	/** NPCCombatInterface, return token */
	UFUNCTION(Category = "Player")
	virtual void ReturnAttackToken_Implementation(AActor* RequestingAttacker, const int Amount) override;

	/** NPCCombatInterface, return team number */
	UFUNCTION(Category = "Player")
	virtual int32 GetTeamNumber_Implementation() override;

	/** NPCCombatInterface, take damage function */
	UFUNCTION(Category = "Player")
	virtual bool TakeDamage_Implementation(AActor* Attacker, const FSDamageInfo& DamageInfo) override;

	/** NPCCombatInterface, return max health of player */
	UFUNCTION(BlueprintCallable, Category = "Player")
	virtual float GetMaxHealth_Implementation() override;

	/** NPCCombatInterface, return current health of player */
	UFUNCTION(BlueprintCallable, Category = "Player")
	virtual float GetCurrentHealth_Implementation() override;
};


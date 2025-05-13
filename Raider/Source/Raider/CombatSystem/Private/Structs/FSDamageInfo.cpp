#include "Structs/FSDamageInfo.h"

FSDamageInfo::FSDamageInfo()
	: Amount(0.0f),
	  DamageType(EDamageType::None),
	  DamageReact(EDamageReact::None),
	  ShouldDamageInvisible(false),
	  ShouldForceInterrupt(false),
	  CanBeBlocked(false)
{
}

FSDamageInfo::FSDamageInfo(float InAmount)
	: Amount(InAmount),
	  DamageType(EDamageType::None),
	  DamageReact(EDamageReact::None),
	  ShouldDamageInvisible(false),
	  ShouldForceInterrupt(false),
	  CanBeBlocked(false)
{
}
	
FSDamageInfo::FSDamageInfo(float InAmount, EDamageType InDamageType, EDamageReact InDamageReact, bool InShouldDamageInvisible,
	bool InShouldForceInterrupt, bool InCanBeBlocked)
	: Amount(InAmount),
	  DamageType(InDamageType),
	  DamageReact(InDamageReact),
	  ShouldDamageInvisible(InShouldDamageInvisible),
	  ShouldForceInterrupt(InShouldForceInterrupt),
	  CanBeBlocked(InCanBeBlocked)
{
}

FString FSDamageInfo::ToString() const
{
	return FString::Printf(TEXT("Damage: %.2f, Type: %d, React: %d, Invisible: %s, Interrupt: %s, Blocked: %s"),
		Amount,
		static_cast<uint8>(DamageType),
		static_cast<uint8>(DamageReact),
		ShouldDamageInvisible ? TEXT("True") : TEXT("False"),
		ShouldForceInterrupt ? TEXT("True") : TEXT("False"),
		CanBeBlocked ? TEXT("True") : TEXT("False"));
}

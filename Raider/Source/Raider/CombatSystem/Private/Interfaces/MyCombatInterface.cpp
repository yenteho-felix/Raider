// Game Development Exercise by Felix Ho


#include "Interfaces/MyCombatInterface.h"

void IMyCombatInterface::GetCombatRange_Implementation(float& OutAttackRadius, float& OutDefendRadius)
{
	OutAttackRadius = 100.f;  // Default values
	OutDefendRadius = 200.f;
}
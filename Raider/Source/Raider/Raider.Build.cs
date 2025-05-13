// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Raider : ModuleRules
{
	public Raider(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
	        { 
		        "Core", "CoreUObject", "Engine", "InputCore", 
		        "NavigationSystem", 
		        "AIModule", 
		        "Niagara", 
		        "EnhancedInput",
		        "UMG"
	        });
        
        PublicIncludePaths.AddRange(new string[] 
	        {
		        "Raider",
		        "Raider/CombatSystem",
		        "Raider/CombatSystem/Public"
	        }
        );
        
        OptimizeCode = CodeOptimization.Never;  // remove from real game
    }
}

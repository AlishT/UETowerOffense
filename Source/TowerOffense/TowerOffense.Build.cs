// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TowerOffense : ModuleRules
{
    public TowerOffense(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "EnhancedInput",
            "AIModule",
            "GameplayTasks",
            "NetCore"
        });
    }

}


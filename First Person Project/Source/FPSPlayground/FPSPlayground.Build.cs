// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSPlayground : ModuleRules
{
	public FPSPlayground(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks", "CableComponent", "UMG", "Slate", "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "CableComponent" });
    }
}

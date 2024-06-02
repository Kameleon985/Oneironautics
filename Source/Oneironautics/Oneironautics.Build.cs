// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Oneironautics : ModuleRules
{
	public Oneironautics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PublicIncludePaths.Add("Oneironautics/");

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}

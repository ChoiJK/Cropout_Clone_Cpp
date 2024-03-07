// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cropout_Clone_Cpp : ModuleRules
{
	public Cropout_Clone_Cpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "EnhancedInput", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new[] { "GeometryScriptingCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
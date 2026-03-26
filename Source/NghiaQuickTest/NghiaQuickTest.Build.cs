// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NghiaQuickTest : ModuleRules
{
	public NghiaQuickTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"NghiaQuickTest",
			"NghiaQuickTest/GAS",
			"NghiaQuickTest/GAS/Attributes",
			"NghiaQuickTest/GAS/Abilities",
			"NghiaQuickTest/GAS/Effects",
			"NghiaQuickTest/InteractiveObject",
			"NghiaQuickTest/Subsystem",
			"NghiaQuickTest/Variant_Platforming",
			"NghiaQuickTest/Variant_Platforming/Animation",
			"NghiaQuickTest/Variant_Combat",
			"NghiaQuickTest/Variant_Combat/AI",
			"NghiaQuickTest/Variant_Combat/Animation",
			"NghiaQuickTest/Variant_Combat/Gameplay",
			"NghiaQuickTest/Variant_Combat/Interfaces",
			"NghiaQuickTest/Variant_Combat/UI",
			"NghiaQuickTest/Variant_SideScrolling",
			"NghiaQuickTest/Variant_SideScrolling/AI",
			"NghiaQuickTest/Variant_SideScrolling/Gameplay",
			"NghiaQuickTest/Variant_SideScrolling/Interfaces",
			"NghiaQuickTest/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

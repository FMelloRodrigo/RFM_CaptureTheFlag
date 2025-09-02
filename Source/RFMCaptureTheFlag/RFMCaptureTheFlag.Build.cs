// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RFMCaptureTheFlag : ModuleRules
{
	public RFMCaptureTheFlag(ReadOnlyTargetRules Target) : base(Target)
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
			"OnlineSubsystemSteam",
            "OnlineSubsystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"RFMCaptureTheFlag",
			"RFMCaptureTheFlag/Variant_Shooter",
			"RFMCaptureTheFlag/Variant_Shooter/AI",
			"RFMCaptureTheFlag/Variant_Shooter/UI",
			"RFMCaptureTheFlag/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

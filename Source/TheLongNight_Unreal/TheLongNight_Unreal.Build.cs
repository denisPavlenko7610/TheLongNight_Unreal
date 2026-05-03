using UnrealBuildTool;

public class TheLongNight_Unreal : ModuleRules
{
	public TheLongNight_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
		[
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		]
		);

		PrivateDependencyModuleNames.AddRange([]);

		PublicIncludePaths.AddRange(
		[
			"TheLongNight_Unreal",
			"TheLongNight_Unreal/Systems/",
			"TheLongNight_Unreal/Systems/PickUp"
		]
		);

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(["Slate", "SlateCore", "Engine", "CoreUObject"]);

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

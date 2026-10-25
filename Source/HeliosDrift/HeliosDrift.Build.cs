using UnrealBuildTool;

public class HeliosDrift : ModuleRules
{
	public HeliosDrift(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NetCore", "HTTP", "Json", "JsonUtilities", "HeadMountedDisplay"
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils", "XRBase" });
	}
}

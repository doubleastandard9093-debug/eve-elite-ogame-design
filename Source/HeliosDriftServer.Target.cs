using UnrealBuildTool;
using System.Collections.Generic;

public class HeliosDriftServerTarget : TargetRules
{
	public HeliosDriftServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("HeliosDrift");
	}
}

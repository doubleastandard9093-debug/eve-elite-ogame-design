using UnrealBuildTool;
using System.Collections.Generic;

public class HeliosDriftTarget : TargetRules
{
	public HeliosDriftTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("HeliosDrift");
	}
}

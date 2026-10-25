using UnrealBuildTool;
using System.Collections.Generic;

public class HeliosDriftClientTarget : TargetRules
{
	public HeliosDriftClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("HeliosDrift");
	}
}

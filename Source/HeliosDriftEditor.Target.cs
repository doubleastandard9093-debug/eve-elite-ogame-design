using UnrealBuildTool;
using System.Collections.Generic;

public class HeliosDriftEditorTarget : TargetRules
{
	public HeliosDriftEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("HeliosDrift");
	}
}

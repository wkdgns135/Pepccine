// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pepccine : ModuleRules
{
	public Pepccine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(
            new string[] {
                "Pepccine",
                "Pepccine/Item",
                "Pepccine/Item/Data"
            }
        );

        //PrivateIncludePaths.Add("Pepccine");
    }
}

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EqEngine : ModuleRules
{
	public EqEngine(TargetInfo Target)
	{
        //MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "OnlineSubsystem",
            "OnlineSubsystemUtils" }); 
        PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}

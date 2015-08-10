// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UE4Sample : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }
    public bool LoadNebuleuse(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string ConfigVersion = (Target.Configuration == UnrealTargetConfiguration.Debug && BuildConfiguration.bDebugBuildsActuallyUseDebugCRT) ? "Debug" : "Release";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "nebuleuse", "lib", ConfigVersion);

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "Nebuleuse." + PlatformString + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libcurl_a." + PlatformString + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "nebuleuse", "include"));
        }
        
        if(isLibrarySupported)
            Definitions.Add("NEBULEUSE");

        return isLibrarySupported;
    }

	public UE4Sample(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        LoadNebuleuse(Target);
	}
}

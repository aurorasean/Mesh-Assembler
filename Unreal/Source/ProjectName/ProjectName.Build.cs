// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ProjectName : ModuleRules
{
    public ProjectName(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "KismetCompiler", "AssetRegistry", "Kismet", "Python3" });
    }
}

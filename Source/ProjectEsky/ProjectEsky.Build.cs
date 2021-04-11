// Fill out your copyright notice in the Description page of Project Settings.
using System;
using System.IO;
using UnrealBuildTool;

public class ProjectEsky : ModuleRules
{
	public ProjectEsky(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore","JsonUtilities","Json" });
		PublicIncludePaths.AddRange(new string[] {});
		AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11");
		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}

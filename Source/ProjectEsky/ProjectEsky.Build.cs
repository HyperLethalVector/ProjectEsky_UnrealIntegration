// Fill out your copyright notice in the Description page of Project Settings.
using System;
using System.IO;
using UnrealBuildTool;

public class ProjectEsky : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}
	private string OutputPlayPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/Win64/realsense2.dll")); }
	}
	private string RealsenseBinaryPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Plugins/realsense2.dll")); }
	}
	public ProjectEsky(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI",
				"RenderCore" });
		PrivateIncludePaths.Add("Runtime/Windows/D3D11RHI/Private");
		PublicIncludePaths.AddRange(new string[] {
		"Runtime/Windows/D3D11RHI/Public",
		"Runtime/Windows/D3D11RHI/Private",
		"Runtime/Windows/D3D11RHI/Private/Windows",
		});
		AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11");
		PrivateDependencyModuleNames.AddRange(new string[] {  });
//		Console.WriteLine("Binaries: " + BinariesPath);
//		Console.WriteLine("Modules: " + ModulePath);
//		Directory.CreateDirectory(OutputPlayPath);
		if(!File.Exists(RealsenseBinaryPath))
		File.Copy(RealsenseBinaryPath,OutputPlayPath);
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class EskyLibrary : ModuleRules
{


	private string ModulePath
	{
		get { return ModuleDirectory; }
	}
	private string BinariesPath
	{
		get
		{
			return Path.GetFullPath(Path.Combine(ModulePath, "../../../Binaries/"));
		}
	}
	private int HashFile(string FilePath)
	{
		string DLLString = File.ReadAllText(FilePath);
		return DLLString.GetHashCode() + DLLString.Length;  //ensure both hash and file lengths match
	}

	public string GetUProjectPath()
	{
		return Path.Combine(ModuleDirectory, "../../../../..");
	}
	private void CopyToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
	{
		//System.Console.WriteLine("uprojectpath is: " + Path.GetFullPath(GetUProjectPath()));

		string BinariesDir = Path.Combine(GetUProjectPath(), "Binaries", Target.Platform.ToString());
		string Filename = Path.GetFileName(Filepath);

		//convert relative path 
		string FullBinariesDir = Path.GetFullPath(BinariesDir);

		if (!Directory.Exists(FullBinariesDir))
		{
			Directory.CreateDirectory(FullBinariesDir);
		}

		string FullExistingPath = Path.Combine(FullBinariesDir, Filename);
		bool ValidFile = false;

		//File exists, check if they're the same
		if (File.Exists(FullExistingPath))
		{
			int ExistingFileHash = HashFile(FullExistingPath);
			int TargetFileHash = HashFile(Filepath);
			ValidFile = ExistingFileHash == TargetFileHash;
			if (!ValidFile)
			{
				System.Console.WriteLine("Project Esky: outdated dlls detected.");
			}
		}

		//No valid existing file found, copy new dll
		if (!ValidFile)
		{
			System.Console.WriteLine("Project Esky: Copied from " + Filepath + ", to " + Path.Combine(FullBinariesDir, Filename));
			File.Copy(Filepath, Path.Combine(FullBinariesDir, Filename), true);
		}
	}
	public EskyLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		string PlatformString = Target.Platform.ToString();
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
//			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ExampleLibrary.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("TextureShareSDK.dll");
			PublicDelayLoadDLLs.Add("libProjectEskyLLAPIIntel.dll");
			PublicDelayLoadDLLs.Add("ProjectEskyLLAPIRenderer.dll");
			PublicDelayLoadDLLs.Add("realsense2.dll");
			System.Console.WriteLine("Copying " + BinariesPath);
			string PluginDLLPath1 = Path.Combine(BinariesPath, PlatformString, "TextureShareSDK.dll");
			string PluginDLLPath2 = Path.Combine(BinariesPath, PlatformString, "libProjectEskyLLAPIIntel.dll");
			string PluginDLLPath3 = Path.Combine(BinariesPath, PlatformString, "ProjectEskyLLAPIRenderer.dll");
			string PluginDLLPath4 = Path.Combine(BinariesPath, PlatformString, "realsense2.dll");
			string PluginShaderPath = Path.Combine(BinariesPath, PlatformString, "shaders.shader");

			System.Console.WriteLine("Project plugin detected, using dll at " + PluginDLLPath3);

			//For project plugins, copy the dll to the project if needed
			CopyToProjectBinaries(PluginDLLPath1, Target);
			CopyToProjectBinaries(PluginDLLPath2, Target);
			CopyToProjectBinaries(PluginDLLPath3, Target);
			CopyToProjectBinaries(PluginDLLPath4, Target);
			CopyToProjectBinaries(PluginShaderPath, Target);


			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/EskyLibrary/Win64/TextureShareSDK.dll");
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/EskyLibrary/Win64/libProjectEskyLLAPIIntel.dll");
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/EskyLibrary/Win64/ProjectEskyLLAPIRenderer.dll");
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/EskyLibrary/Win64/realsense2.dll");
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/EskyLibrary/Win64/shaders.shader");
		}
	}
}

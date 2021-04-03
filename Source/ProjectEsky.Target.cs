// Fill out your copyright notice in the Description page of Project Settings.
using System;
using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;
public class ProjectEskyTarget : TargetRules
{

//	private string OutputPlayPath
//	{
//		get { return Path.GetFullPath(Path.Combine(ProjectFile, "../Binaries/Win64/realsense2.dll")); }
//	}
//	private string RealsenseBinaryPath
//	{
//		get { return Path.GetFullPath(Path.Combine(ProjectFile, "../Plugins/realsense2.dll")); }
//	}
	public ProjectEskyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ProjectEsky" } );
//		File.Copy(RealsenseBinaryPath,OutputPlayPath);
		
	}
}

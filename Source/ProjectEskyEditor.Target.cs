// Fill out your copyright notice in the Description page of Project Settings.
using System;
using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;
public class ProjectEskyEditorTarget : TargetRules
{
	
	public ProjectEskyEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ProjectEsky" } );
		
		//Console.WriteLine(System.AppDomain.CurrentDomain.BaseDirectory);
	}
}

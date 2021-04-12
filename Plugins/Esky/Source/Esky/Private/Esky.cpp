// Copyright Epic Games, Inc. All Rights Reserved.

#include "Esky.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FEskyModule"
void* FEskyModule::GetTrackerDLLHandle()
{
	void* NewTrackerDLLHandle = nullptr;


	FString BaseDir = IPluginManager::Get().FindPlugin("Esky")->GetBaseDir();
	FString BinariesPath = BaseDir / FString(TEXT("Binaries/Win64"));
	FPlatformProcess::PushDllDirectory(*BinariesPath);
	NewTrackerDLLHandle = FPlatformProcess::GetDllHandle(*(BinariesPath / "libProjectEskyLLAPIIntel.dll"));
	FPlatformProcess::PopDllDirectory(*BinariesPath);

	if (NewTrackerDLLHandle != nullptr)
	{
		UE_LOG(EskyLog, Log, TEXT("Esky Tracker plugin DLL found at %s"), *FPaths::ConvertRelativePathToFull(BinariesPath / "libProjectEskyLLAPIIntel.dll"));
	}
	else {
		UE_LOG(EskyLog, Warning, TEXT("Esky Tracker plugin DLL wasn't found at %s"), *FPaths::ConvertRelativePathToFull(BinariesPath / "ProjectEskyLLAPIRenderer.dll"));
	}
	return NewTrackerDLLHandle;
}
void* FEskyModule::GetRendererDLLHandle()
{
	void* NewRendererDLLHandle = nullptr;
	FString BaseDir = IPluginManager::Get().FindPlugin("Esky")->GetBaseDir();
	FString BinariesPath = BaseDir / FString(TEXT("Binaries/Win64"));
	FPlatformProcess::PushDllDirectory(*BinariesPath);
	NewRendererDLLHandle = FPlatformProcess::GetDllHandle(*(BinariesPath / "ProjectEskyLLAPIRenderer.dll"));
	FPlatformProcess::PopDllDirectory(*BinariesPath);

	if (NewRendererDLLHandle != nullptr)
	{
		UE_LOG(EskyLog, Log, TEXT("Esky Render plugin DLL found at %s"), *FPaths::ConvertRelativePathToFull(BinariesPath / "ProjectEskyLLAPIRenderer.dll"));
	}
	else {
		UE_LOG(EskyLog, Warning, TEXT("Esky Render plugin DLL wasn't found at %s"), *FPaths::ConvertRelativePathToFull(BinariesPath / "ProjectEskyLLAPIRenderer.dll"));
	}
	return NewRendererDLLHandle;
}
void FEskyModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TrackerDLLHandle = GetTrackerDLLHandle();
	RendererDLLHandle = GetRendererDLLHandle();
	if (TrackerDLLHandle && RendererDLLHandle)
	{
		// Call the test function in the third party library that opens a message box
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Project Esky", "Loaded Project Esky libraries"));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Project Esky", "Failed to load Project Esky libraries"));
	}
}

void FEskyModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	if (TrackerDLLHandle) {
		FPlatformProcess::FreeDllHandle(TrackerDLLHandle);
		TrackerDLLHandle = nullptr;
	}
	if (RendererDLLHandle) {
		FPlatformProcess::FreeDllHandle(RendererDLLHandle);
		RendererDLLHandle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEskyModule, Esky)

DEFINE_LOG_CATEGORY(EskyLog);
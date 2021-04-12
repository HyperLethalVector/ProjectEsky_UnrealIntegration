// Copyright Epic Games, Inc. All Rights Reserved.

#include "Esky.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FEskyModule"
void DebugMessagePlugin(const wchar_t* message) {
	UE_LOG(EskyLog, Warning, TEXT("Renderer: %s"), message);
}
#pragma region DLL Specific functions
typedef void(*OnRenderedFrameCallbackPlugin)();
typedef void(*_SetOnReceivedFrameCallbackPlugin)(int windowID, OnRenderedFrameCallbackPlugin callback);
typedef void(*_StartWindowByIdPlugin)(int windowID, int width, int height, bool borderless);
typedef void(*_StopWindowByIdPlugin)(int windowID);
typedef void(*_SetWindowRectByIdPlugin)(int windowID, int left, int top, int width, int height);
typedef void(*_SetDeltasPlugin)(int windowID, float* deltaLeft, float* deltaRight);
typedef void(*_InitializeGraphicsPlugin)(int windowID);
typedef void(*_SetEnableFlagWarpingPlugin)(int windowID, bool enabled);
typedef void(*_SetRequiredValuesByIdPlugin)(int windowID, float* leftUvToRectX, float* leftUvToRectY, float* rightUvToRectX, float* rightUvToRectY, float* CameraMatrixLeft, float* CameraMatrixRight, float* InvCameraMatrixLeft, float* InvCameraMatrixRight, float* leftOffset, float* rightOffset, float* eyeBorders);
typedef void(*_SetColorFormatPlugin)(int colorFormat);
typedef void(*_SetBrightnessPlugin)(int windowID, float brightness);

typedef void (*FuncPtrPlugin) (const wchar_t*);
typedef void(*_SetDebugFunctionPlugin)(int WindowID, void* fp);
typedef void(*_FreeDebugFunctionPlugin)();
#pragma endregion
#pragma region DLL function storage variables
_StartWindowByIdPlugin m_StartWindowByIdPlugin;
_StopWindowByIdPlugin m_StopWindowByIdPlugin;
_SetWindowRectByIdPlugin m_SetWindowRectByIdPlugin;
_SetDeltasPlugin m_SetDeltasPlugin;
_InitializeGraphicsPlugin m_InitializeGraphicsPlugin;
_SetEnableFlagWarpingPlugin m_SetEnableFlagWarpingPlugin;
_SetRequiredValuesByIdPlugin m_SetRequiredValuesByIdPlugin;
_SetBrightnessPlugin m_SetBrightnessPlugin;
_SetDebugFunctionPlugin m_SetDebugFunctionPlugin;
_FreeDebugFunctionPlugin m_FreeDebugFunctionPlugin;
_SetColorFormatPlugin m_SetColorFormatPlugin;
_SetOnReceivedFrameCallbackPlugin m_SetOnReceivedFrameCallbackPlugin;
#pragma endregion
FuncPtrPlugin ptrPlugin;

void FEskyModule::LoadConfig(FString fileName) {
	FString file = FPaths::ProjectConfigDir();
	file.Append(fileName);
	FString resultingJson;
	UE_LOG(LogTemp, Warning, TEXT("Loading file: %s"), *file);
	FFileHelper::LoadFileToString(resultingJson, *file);
	if (FJsonObjectConverter::JsonObjectStringToUStruct(resultingJson, &myInfo, 0, 0))
	{
		UE_LOG(EskyLog, Warning, TEXT("Loaded config successfully"));
	}
	else {
		UE_LOG(EskyLog, Warning, TEXT("Didn't load config successfully"));
	}
}
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
void FEskyModule::StartRendererComponent() {
	m_StartWindowByIdPlugin = (_StartWindowByIdPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("StartWindowById"));
	m_StopWindowByIdPlugin = (_StopWindowByIdPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("StopWindowById"));
	m_SetWindowRectByIdPlugin = (_SetWindowRectByIdPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetWindowRectById"));
	m_SetDeltasPlugin = (_SetDeltasPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetDeltas"));
	m_InitializeGraphicsPlugin = (_InitializeGraphicsPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("InitializeGraphics"));
	m_SetEnableFlagWarpingPlugin = (_SetEnableFlagWarpingPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetEnableFlagWarping"));
	m_SetRequiredValuesByIdPlugin = (_SetRequiredValuesByIdPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetRequiredValuesById"));
	m_SetBrightnessPlugin = (_SetBrightnessPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetBrightness"));
	m_SetDebugFunctionPlugin = (_SetDebugFunctionPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetDebugFunction"));
	m_FreeDebugFunctionPlugin = (_FreeDebugFunctionPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("FreeDebugFunction"));
	m_SetColorFormatPlugin = (_SetColorFormatPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetColorFormat"));
	m_SetOnReceivedFrameCallbackPlugin = (_SetOnReceivedFrameCallbackPlugin)FPlatformProcess::GetDllExport(RendererDLLHandlePlugin, *FString("SetOnReceivedFrameCallback"));
	LoadConfig("DataConfig.json");
	for (int i = 0; i < 16; i++) {
		LeftUVToRectX[i] = myInfo.LeftUVToRectX[i];
		LeftUVToRectY[i] = myInfo.LeftUVToRectY[i];
		RightUVToRectX[i] = myInfo.RightUVToRectX[i];
		RightUVToRectY[i] = myInfo.RightUVToRectY[i];
	}
	width = myInfo.WindowWidth;
	height = myInfo.WindowHeight;
	xPlacement = myInfo.WindowOffsetX;
	yPlacement = myInfo.WindowOffsetY;
	useTemporalReprojection = myInfo.useTemporalReprojection;

	FuncPtrPlugin fp = &DebugMessagePlugin;
	m_SetDebugFunctionPlugin(WindowID, fp);
	UE_LOG(EskyLog, Warning, TEXT("Attached the debug message"));
	m_StartWindowByIdPlugin(WindowID, width, height, true);
	UE_LOG(EskyLog, Warning, TEXT("Started Window"));
	m_SetColorFormatPlugin(0);
	UE_LOG(EskyLog, Warning, TEXT("Set received frame callback"));
	m_SetEnableFlagWarpingPlugin(WindowID, useTemporalReprojection);
	UE_LOG(EskyLog, Warning, TEXT("Init Graphics"));
	m_InitializeGraphicsPlugin(WindowID);
	m_SetRequiredValuesByIdPlugin(WindowID, LeftUVToRectX, LeftUVToRectY, RightUVToRectX, RightUVToRectY, LeftEyeProjectionMatrix, RightEyeProjectionMatrix, LeftEyeInvProjectionMatrix, RightEyeInvProjectionMatrix, LeftOffset, RightOffset, eyeBorders);
	UE_LOG(EskyLog, Warning, TEXT("Set Window Position"));
	m_SetWindowRectByIdPlugin(WindowID, xPlacement, yPlacement, width, height);
}
void FEskyModule::StopRendererComponent() {
	m_SetDebugFunctionPlugin(WindowID, nullptr);
	UE_LOG(EskyLog, Warning, TEXT("Stopping renderer!"));
	m_StopWindowByIdPlugin(WindowID);
}
void FEskyModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TrackerDLLHandlePlugin = GetTrackerDLLHandle();
	RendererDLLHandlePlugin = GetRendererDLLHandle();
	if (TrackerDLLHandlePlugin && RendererDLLHandlePlugin)
	{
		StartRendererComponent();
		// Call the test function in the third party library that opens a message box
		//FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Project Esky", "Loaded Project Esky libraries"));
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
	if (TrackerDLLHandlePlugin) {
		FPlatformProcess::FreeDllHandle(TrackerDLLHandlePlugin);
		TrackerDLLHandlePlugin = nullptr;
	}
	if (RendererDLLHandlePlugin) {
		StopRendererComponent();
		m_StartWindowByIdPlugin = NULL;
		m_StopWindowByIdPlugin = NULL;
		m_SetWindowRectByIdPlugin = NULL;
		m_SetDeltasPlugin = NULL;
		m_InitializeGraphicsPlugin = NULL;
		m_SetEnableFlagWarpingPlugin = NULL;
		m_SetRequiredValuesByIdPlugin = NULL;
		m_SetBrightnessPlugin = NULL;
		m_SetDebugFunctionPlugin = NULL;
		m_FreeDebugFunctionPlugin = NULL;
		m_SetColorFormatPlugin = NULL;
		m_SetOnReceivedFrameCallbackPlugin = NULL;
		FPlatformProcess::FreeDllHandle(RendererDLLHandlePlugin);
		RendererDLLHandlePlugin = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEskyModule, Esky)

DEFINE_LOG_CATEGORY(EskyLog);
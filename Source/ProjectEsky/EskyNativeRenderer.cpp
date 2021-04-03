// Fill out your copyright notice in the Description page of Project Settings.
#include "EskyNativeRenderer.h"
#pragma region DLL Specific functions
typedef void(*_StartWindowById)(int windowID, int width,  int height, bool borderless);
typedef void(*_StopWindowById)(int windowID);
typedef void(*_SetWindowRectById)(int windowID, int left, int top, int width, int height);
typedef void(*_SetDeltas)(int windowID, float* deltaLeft, float* deltaRight);
typedef void(*_InitializeGraphics)(int windowID);
typedef void(*_SetEnableFlagWarping)(int windowID, bool enabled);
typedef void(*_SetRequiredValuesById)(int windowID, float* leftUvToRectX, float* leftUvToRectY, float* rightUvToRectX, float* rightUvToRectY, float* CameraMatrixLeft, float* CameraMatrixRight, float* InvCameraMatrixLeft, float* InvCameraMatrixRight, float* leftOffset, float* rightOffset, float* eyeBorders);
typedef void(*_SetBrightness)(int windowID, float brightness);
typedef void (*FuncPtr) (const char *);
typedef void(*_SetDebugFunction)(FuncPtr fp);
#pragma endregion
#pragma region DLL function storage variables
_StartWindowById m_StartWindowById;
_StopWindowById m_StopWindowById;
_SetWindowRectById m_SetWindowRectById;
_SetDeltas m_SetDeltas;
_InitializeGraphics m_InitializeGraphics;
_SetEnableFlagWarping m_SetEnableFlagWarping;
_SetRequiredValuesById m_SetRequiredValuesById;
_SetBrightness m_SetBrightness;
_SetDebugFunction m_SetDebugFunction;
#pragma endregion
void *v_dllHandle_renderer;

UEskyNativeRenderer* rendererInstance;
bool UEskyNativeRenderer::importDLL()
{
    FString filePath = *FPaths::ProjectPluginsDir() + FString("libProjectEskyLLAPIIntel.dll");
    UE_LOG(LogTemp, Warning, TEXT("Location of plugin: %s"),*filePath);   
    if (FPaths::FileExists(filePath))
    {         
        v_dllHandle_renderer = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle_renderer != NULL)
        {
			m_StartWindowById = (_StartWindowById)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("StartWindowById"));
            m_StopWindowById = (_StopWindowById)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("StopWindowById"));
			m_SetWindowRectById = (_SetWindowRectById)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetWindowRectById"));
			m_SetDeltas= (_SetDeltas)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetDeltas"));
			m_InitializeGraphics = (_InitializeGraphics)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("InitializeGraphics"));
			m_SetEnableFlagWarping= (_SetEnableFlagWarping)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetEnableFlagWarping"));
			m_SetRequiredValuesById= (_SetRequiredValuesById)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetRequiredValuesById"));
			m_SetBrightness= (_SetBrightness)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetBrightness"));
			m_SetDebugFunction = (_SetDebugFunction)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetDebugFunction"));
			return true;
		}
	}
	return false;
}
void UEskyNativeRenderer::freeDLL(){
	if(v_dllHandle_renderer != NULL){
			m_StartWindowById = NULL;
            m_StopWindowById = NULL;
			m_SetWindowRectById = NULL;
			m_SetDeltas= NULL;
			m_InitializeGraphics = NULL;
			m_SetEnableFlagWarping= NULL;
			m_SetRequiredValuesById= NULL;
			m_SetBrightness= NULL;
			FPlatformProcess::FreeDllHandle(v_dllHandle_renderer);
			v_dllHandle_renderer = NULL;
	}
}
// Sets default values for this component's properties
UEskyNativeRenderer::UEskyNativeRenderer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEskyNativeRenderer::DebugMessage(const char* message){
	UE_LOG(LogTemp, Warning, TEXT("Renderer: %s"),message);
}
// Called when the game starts
void UEskyNativeRenderer::BeginPlay()
{
	Super::BeginPlay();
	successful = importDLL();
	if(successful){
        UE_LOG(LogTemp, Warning, TEXT("DLL Loaded, Started Renderer!"));  
//		FuncPtr fp = &DebugMessage;	
//		m_SetDebugFunction(fp);
		m_StartWindowById(WindowID,width,height,true);      		
	}else{
        UE_LOG(LogTemp, Warning, TEXT("Renderer DLL wasn't loaded"));     
	}

//	FD3D11DynamicRHI* D3D11RHIPtr = Cast<FD3D11DynamicRHI>(GDynamicRHI);
//	ID3D11Device* DevicePtr = D3D11RHIPtr->GetDevice();
	// ...
	
}
void UEskyNativeRenderer::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);
	if(successful){
		rendererInstance = NULL;				
        UE_LOG(LogTemp, Warning, TEXT("Stopping renderer!"));                        
        m_StopWindowById(WindowID);
        freeDLL();
        UE_LOG(LogTemp, Warning, TEXT("Free'd the DLL!"));                                
    }
}

// Called every frame
void UEskyNativeRenderer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(successful){

	}
	// ...
}


// Fill out your copyright notice in the Description page of Project Settings.
#include "EskyNativeRenderer.h"
#include <string>
#include <locale>
#include "IntelRealsenseTracker.h"

void DebugMessage(const wchar_t* message){
	UE_LOG(LogTemp, Warning, TEXT("Renderer: %s"),message);
}
#pragma region DLL Specific functions
typedef void(*OnRenderedFrameCallback)();
typedef void(*_SetOnReceivedFrameCallback)(int windowID, OnRenderedFrameCallback callback);
typedef void(*_StartWindowById)(int windowID, int width,  int height, bool borderless);
typedef void(*_StopWindowById)(int windowID);
typedef void(*_SetWindowRectById)(int windowID, int left, int top, int width, int height);
typedef void(*_SetDeltas)(int windowID, float* deltaLeft, float* deltaRight);
typedef void(*_InitializeGraphics)(int windowID);
typedef void(*_SetEnableFlagWarping)(int windowID, bool enabled);
typedef void(*_SetRequiredValuesById)(int windowID, float* leftUvToRectX, float* leftUvToRectY, float* rightUvToRectX, float* rightUvToRectY, float* CameraMatrixLeft, float* CameraMatrixRight, float* InvCameraMatrixLeft, float* InvCameraMatrixRight, float* leftOffset, float* rightOffset, float* eyeBorders);
typedef void(*_SetColorFormat)(int colorFormat);
typedef void(*_SetBrightness)(int windowID, float brightness);

typedef void (*FuncPtr) (const wchar_t *);
typedef void(*_SetDebugFunction)(void *fp);
typedef void(*_FreeDebugFunction)();
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
_FreeDebugFunction m_FreeDebugFunction;
_SetColorFormat m_SetColorFormat;
_SetOnReceivedFrameCallback m_SetOnReceivedFrameCallback;
#pragma endregion
void *v_dllHandle_renderer;
FuncPtr ptr;
bool isDone = false;

UEskyNativeRenderer* rendererInstance;
void UEskyNativeRenderer::SetDeltas(int ID, float* leftEye, float* rightEye){

	if(rendererInstance != nullptr){
		if(rendererInstance->successful){		
				rendererInstance->SetDeltasLocal(ID,leftEye,rightEye);
		}
	}
//	UEskyNativeRenderer* rendererInstance;
};
bool UEskyNativeRenderer::importDLL()
{
    FString filePath = *FPaths::ProjectPluginsDir() + FString("ProjectEskyLLAPIRenderer.dll");
    UE_LOG(LogTemp, Warning, TEXT("Location of plugin: %s"),*filePath);   
    if (FPaths::FileExists(filePath))
    {         
		if(!isDone){

			isDone = true;
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
				m_FreeDebugFunction =(_FreeDebugFunction)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("FreeDebugFunction"));
				m_SetColorFormat = (_SetColorFormat)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetColorFormat"));
				m_SetOnReceivedFrameCallback = (_SetOnReceivedFrameCallback)FPlatformProcess::GetDllExport(v_dllHandle_renderer, *FString("SetOnReceivedFrameCallback"));
				return true;
			}
		}
	}
	return false;
}
void UEskyNativeRenderer::freeDLL(){
	if(v_dllHandle_renderer != NULL){
			UE_LOG(LogTemp, Warning, TEXT("Freeing the DLL!")); 
			m_StartWindowById = NULL;
            m_StopWindowById = NULL;
			m_SetWindowRectById = NULL;
			m_SetDeltas= NULL;
			m_InitializeGraphics = NULL;
			m_SetEnableFlagWarping= NULL;
			m_SetRequiredValuesById= NULL;
			m_SetBrightness= NULL;
			m_SetDebugFunction = NULL;	
			m_FreeDebugFunction = NULL;			
			m_SetColorFormat = NULL;
			m_SetOnReceivedFrameCallback = NULL;
			v_dllHandle_renderer = NULL;

			FPlatformProcess::FreeDllHandle(v_dllHandle_renderer);
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
void UEskyNativeRenderer::SetDeltasLocal(int iD, float* leftEye, float* rightEye){
	m_SetDeltas(iD,leftEye,rightEye);
}
// Called when the game starts
void UEskyNativeRenderer::BeginPlay()
{
	Super::BeginPlay();
	

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
		UE_LOG(LogTemp, Warning, TEXT("Freeing the DLL!"));                        
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

void UEskyNativeRenderer::SetAttachedTracker(UIntelRealsenseTracker* trackerToAttach){       
    UE_LOG(LogTemp, Warning, TEXT("Attached the Tracker!"));  
    successful = importDLL();
    myAttachedTracker = trackerToAttach;
	if(successful){
        UE_LOG(LogTemp, Warning, TEXT("DLL Loaded, Started Renderer!"));  
		FuncPtr fp = &DebugMessage;	
		m_SetDebugFunction(fp);
		rendererInstance = this;
		m_StartWindowById(WindowID,width,height,true);      	
		m_SetColorFormat(0);
		OnRenderedFrameCallback orfc = &UIntelRealsenseTracker::RenderedFrameCallback;
        m_SetOnReceivedFrameCallback(WindowID,orfc);
		m_SetEnableFlagWarping(WindowID,useTemporalReprojection);
		m_InitializeGraphics(WindowID);		
		m_SetRequiredValuesById(WindowID,LeftUVToRectX,LeftUVToRectY,RightUVToRectX,RightUVToRectY,LeftEyeProjectionMatrix,RightEyeProjectionMatrix,LeftEyeInvProjectionMatrix,RightEyeInvProjectionMatrix,LeftOffset,RightOffset,eyeBorders);			
		m_SetWindowRectById(WindowID,xPlacement,yPlacement,width,height);

	}else{
        UE_LOG(LogTemp, Warning, TEXT("Renderer DLL wasn't loaded"));     
	}
}


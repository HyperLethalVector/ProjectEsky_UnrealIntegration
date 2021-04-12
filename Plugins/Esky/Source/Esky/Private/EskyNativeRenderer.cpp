// Fill out your copyright notice in the Description page of Project Settings.
#include "EskyNativeRenderer.h"
#include "Esky.h"
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
typedef void(*_SetDebugFunction)(int WindowID, void *fp);
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
	if (RendererDLLHandle != NULL)
	{
		m_StartWindowById = (_StartWindowById)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("StartWindowById"));
		m_StopWindowById = (_StopWindowById)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("StopWindowById"));
		m_SetWindowRectById = (_SetWindowRectById)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetWindowRectById"));
		m_SetDeltas= (_SetDeltas)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetDeltas"));
		m_InitializeGraphics = (_InitializeGraphics)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("InitializeGraphics"));
		m_SetEnableFlagWarping= (_SetEnableFlagWarping)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetEnableFlagWarping"));
		m_SetRequiredValuesById= (_SetRequiredValuesById)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetRequiredValuesById"));
		m_SetBrightness= (_SetBrightness)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetBrightness"));
		m_SetDebugFunction = (_SetDebugFunction)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetDebugFunction"));
		m_FreeDebugFunction =(_FreeDebugFunction)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("FreeDebugFunction"));
		m_SetColorFormat = (_SetColorFormat)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetColorFormat"));
		m_SetOnReceivedFrameCallback = (_SetOnReceivedFrameCallback)FPlatformProcess::GetDllExport(RendererDLLHandle, *FString("SetOnReceivedFrameCallback"));
		return true;
	}
	return false;
}
void UEskyNativeRenderer::freeDLL(){
	if(RendererDLLHandle != NULL){
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
void UEskyNativeRenderer::CloseRenderer(){
	if(successful){
		rendererInstance = NULL;	
		m_SetOnReceivedFrameCallback(WindowID,nullptr);
		m_SetDebugFunction(WindowID,nullptr);  		
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
    UE_LOG(LogTemp, Warning, TEXT("Imported the DLL!"));  
    myAttachedTracker = trackerToAttach;
	if(successful){
		FuncPtr fp = &DebugMessage;	

		m_SetDebugFunction(WindowID,fp);
	    UE_LOG(LogTemp, Warning, TEXT("Attached the debug message"));  				
		rendererInstance = this;

		m_StartWindowById(WindowID,width,height,true);      	
	    UE_LOG(LogTemp, Warning, TEXT("Started Window"));  				
		m_SetColorFormat(0);
		OnRenderedFrameCallback orfc = &UIntelRealsenseTracker::RenderedFrameCallback;
        m_SetOnReceivedFrameCallback(WindowID,orfc);
	    UE_LOG(LogTemp, Warning, TEXT("Set received frame callback"));  						
		m_SetEnableFlagWarping(WindowID,useTemporalReprojection);
	    UE_LOG(LogTemp, Warning, TEXT("Init Graphics"));  								
		m_InitializeGraphics(WindowID);			
		m_SetRequiredValuesById(WindowID,LeftUVToRectX,LeftUVToRectY,RightUVToRectX,RightUVToRectY,LeftEyeProjectionMatrix,RightEyeProjectionMatrix,LeftEyeInvProjectionMatrix,RightEyeInvProjectionMatrix,LeftOffset,RightOffset,eyeBorders);			
	    UE_LOG(LogTemp, Warning, TEXT("Init Graphics"));  										
		m_SetWindowRectById(WindowID,xPlacement,yPlacement,width,height);

	}else{
        UE_LOG(LogTemp, Warning, TEXT("Renderer DLL wasn't loaded"));     
	}
}
void UEskyNativeRenderer::ApplySettings(UEskyDataContainer* dataContainer){
    for(int i = 0; i < 16; i++){
         LeftUVToRectX[i] = dataContainer->myConfig.LeftUVToRectX[i];
         LeftUVToRectY[i] = dataContainer->myConfig.LeftUVToRectY[i];         
         RightUVToRectX[i] = dataContainer->myConfig.RightUVToRectX[i];
         RightUVToRectY[i] = dataContainer->myConfig.RightUVToRectY[i];      
    }	   
	width = dataContainer->myConfig.WindowWidth;
	height = dataContainer->myConfig.WindowHeight;
	xPlacement = dataContainer->myConfig.WindowOffsetX;
	yPlacement = dataContainer->myConfig.WindowOffsetY;
	useTemporalReprojection = dataContainer->myConfig.useTemporalReprojection;
}
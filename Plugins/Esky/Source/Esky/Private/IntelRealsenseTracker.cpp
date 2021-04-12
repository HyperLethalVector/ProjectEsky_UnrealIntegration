// Fill out your copyright notice in the Description page of Project Settings.
#include "IntelRealsenseTracker.h"
#include "Esky.h"
#include "EskyNativeRenderer.h"
#pragma region DLL callback function definitions
typedef void(*FuncReceiveCameraImageCallbackWithID)(int instanceID, unsigned char* info, int lengthofarray, int width, int height, int pixelCount);
typedef void(*LocalizationEventCallback)(int trackerID, int LocalizationDelegate);
typedef void(*MapDataCallback)(int trackerID);
typedef void(*LocalizationPoseReceivedCallback)(int trackerID, unsigned char* ObjectID, float tx, float ty, float tz, float qx, float qy, float qz, float qw);
typedef void(*DeltaPoseUpdateCallback)(int trackerID, float* poseDataLeft, float* poseDataRight);//needs refactoring into a map callback to subscribe to
typedef void(*FuncTextureInitializedCallback)(int trackerID, int TextureWidth, int TextureHeight, int textureCount, float fx, float fy, float cx, float cy, float fovx, float fovy, float focalLength, float d1, float d2, float d3, float d4, float d5);
#pragma endregion

#pragma region DLL Specific functions

typedef void(*_RegisterDeltaPoseUpdate)(int TrackerID, DeltaPoseUpdateCallback callback);
typedef void(*_HookDeviceToIntel)(int TrackerID);
typedef void(*_SetFilterEnabled)(int iD, bool value); 
typedef void(*_UpdateFilterTranslationParams)(int iD, double _freq, double _mincutoff, double _beta, double _dcutoff);
typedef void(*_UpdateFilterRotationParams)(int iD, double _freq, double _mincutoff, double _beta, double _dcutoff);        
typedef float*(*_GetLatestPose)(int TrackerID);
typedef void(*_InitializeTrackerObject)(int TrackerID);
typedef void(*_SetSerialComPort)(int TrackerID, int port);
typedef void(*_StartTrackerThread)(int TrackerID, bool useLocalization);
typedef void(*_StopTrackers)(int TrackerID);
typedef void(*_RegisterObjectPoseCallback)(int TrackerID, LocalizationPoseReceivedCallback poseReceivedCallback);
typedef void(*_RegisterLocalizationCallback)(int TrackerID, LocalizationEventCallback cb);
typedef void(*_RegisterBinaryMapCallback)(int TrackerID, MapDataCallback cb);
typedef void(*_SetBinaryMapData)(int TrackerID, unsigned char* inputBytesLocation);
typedef void(*_ObtainOriginInLocalizedMap)(int TrackerID);
typedef void(*_ObtainMap)(int TrackerID);
typedef void(*_FlagMapImport)(int TrackerID); 
typedef void(*_EnablePassthrough)(int iD, bool enabled);
typedef void(*_PostRenderReset)(int ID);
typedef void(*_SetLeftRightEyeTransform)(int iD, float* leftEyeTransform, float* rightEyeTransform);
typedef void(*_SetTimeOffset)(int Id, float value);
#pragma endregion
#pragma region DLL function storage variables
_RegisterDeltaPoseUpdate m_RegisterDeltaPoseUpdate;
_HookDeviceToIntel m_HookDeviceToIntel;
_SetFilterEnabled m_SetFilterEnabled;
_UpdateFilterTranslationParams m_UpdateFilterTranslationParams;
_UpdateFilterRotationParams m_UpdateFilterRotationParams;
_GetLatestPose m_GetLatestPose;
_InitializeTrackerObject m_InitializeTrackerObject;
_SetSerialComPort m_SetSerialComPort;
_StartTrackerThread m_StartTrackerThread;
_StopTrackers m_StopTrackers;
_RegisterObjectPoseCallback m_RegisterObjectPoseCallback;
_RegisterLocalizationCallback m_RegisterLocalizationCallback;
_RegisterBinaryMapCallback m_RegisterBinaryMapCallback;
_SetBinaryMapData m_SetBinaryMapData;
_ObtainOriginInLocalizedMap m_ObtainOriginInLocalizedMap;
_ObtainMap m_ObtainMap;
_FlagMapImport m_FlagMapImport;
_EnablePassthrough m_EnablePassthrough;
_PostRenderReset m_PostRenderReset;
_SetLeftRightEyeTransform m_SetLeftRightEyeTransform;
_SetTimeOffset m_SetTimeOffset;

UIntelRealsenseTracker* intelRealsenseInstance;
#pragma endregion


#pragma region Load DLL

// Method to import a DLL.
bool UIntelRealsenseTracker::importDLL()
{
    if (TrackerDLLHandle != NULL)
    {
        //import all the functions

        m_RegisterDeltaPoseUpdate = (_RegisterDeltaPoseUpdate)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("RegisterDeltaPoseUpdate"));
        m_HookDeviceToIntel = (_HookDeviceToIntel)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("HookDeviceToIntel"));
        m_SetFilterEnabled = (_SetFilterEnabled)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("SetFilterEnabled"));
        m_UpdateFilterTranslationParams = (_UpdateFilterTranslationParams)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("UpdateFilterTranslationParams"));
        m_UpdateFilterRotationParams = (_UpdateFilterRotationParams)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("UpdateFilterRotationParams"));
        m_GetLatestPose = (_GetLatestPose)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("GetLatestPose"));
        m_InitializeTrackerObject = (_InitializeTrackerObject)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("InitializeTrackerObject"));
        m_SetSerialComPort = (_SetSerialComPort)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("SetSerialComPort"));
        m_StartTrackerThread = (_StartTrackerThread)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("StartTrackerThread"));
        m_StopTrackers = (_StopTrackers)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("StopTrackers"));
        m_RegisterObjectPoseCallback = (_RegisterObjectPoseCallback)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("RegisterObjectPoseCallback"));
        m_RegisterLocalizationCallback = (_RegisterLocalizationCallback)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("RegisterLocalizationCallback"));
        m_RegisterBinaryMapCallback = (_RegisterBinaryMapCallback)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("RegisterBinaryMapCallback"));
        m_SetBinaryMapData = (_SetBinaryMapData)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("SetBinaryMapData"));
        m_ObtainOriginInLocalizedMap = (_ObtainOriginInLocalizedMap)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("ObtainOriginInLocalizedMap"));
        m_ObtainMap = (_ObtainMap)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("ObtainMap"));
        m_FlagMapImport = (_FlagMapImport)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("FlagMapImport"));
        m_EnablePassthrough = (_EnablePassthrough)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("EnablePassthrough"));
        m_PostRenderReset = (_PostRenderReset)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("PostRenderReset"));
        m_SetLeftRightEyeTransform = (_SetLeftRightEyeTransform)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("SetLeftRightEyeTransform"));
        m_SetTimeOffset = (_SetTimeOffset)FPlatformProcess::GetDllExport(TrackerDLLHandle, *FString("SetTimeOffset"));
        return true;
    }
    return false;    // Return an error.
}
#pragma endregion Load DLL


#pragma region Unload DLL
UIntelRealsenseTracker::UIntelRealsenseTracker(){
    PrimaryComponentTick.bCanEverTick = true;
 //   setInstance(*this);
}
void UIntelRealsenseTracker::freeDLL(){
    if (TrackerDLLHandle != NULL)
    {
        //import all the functions
        intelRealsenseInstance = NULL;
        m_RegisterDeltaPoseUpdate = NULL;
        m_HookDeviceToIntel = NULL;
        m_SetFilterEnabled = NULL;
        m_UpdateFilterTranslationParams = NULL;
        m_UpdateFilterRotationParams = NULL;
        m_GetLatestPose = NULL;
        m_InitializeTrackerObject = NULL;
        m_SetSerialComPort = NULL;
        m_StartTrackerThread = NULL;
        m_StopTrackers = NULL;
        m_RegisterObjectPoseCallback = NULL;
        m_RegisterLocalizationCallback = NULL;
        m_RegisterBinaryMapCallback = NULL;
        m_SetBinaryMapData = NULL;
        m_ObtainOriginInLocalizedMap = NULL;
        m_ObtainMap = NULL;
        m_FlagMapImport = NULL;
        m_EnablePassthrough = NULL;
        m_PostRenderReset = NULL;
        m_SetLeftRightEyeTransform = NULL;
        m_SetTimeOffset = NULL;
    }
}

void UIntelRealsenseTracker::SetAttachedRenderer(UEskyNativeRenderer* rendererToAttach){       
    UE_LOG(LogTemp, Warning, TEXT("Attached the UEskyNativeRenderer!"));  
    successful = importDLL();
    myAttachedRenderer = rendererToAttach;
    intelRealsenseInstance = this;
    if(successful){
        m_InitializeTrackerObject(TrackerID);
        m_SetSerialComPort(TrackerID,5);
        m_StartTrackerThread(TrackerID,false);
        m_SetFilterEnabled(TrackerID,false);
        DeltaPoseUpdateCallback dpuc = &UEskyNativeRenderer::SetDeltas;
        m_RegisterDeltaPoseUpdate(TrackerID,dpuc);
        m_SetLeftRightEyeTransform(TrackerID,leftEyeTransformFromTracker,rightEyeTransformFromTracker);
        UE_LOG(LogTemp, Warning, TEXT("DLL Loaded, Started tracker!"));    
    }else{
        UE_LOG(LogTemp, Warning, TEXT("Tracker DLL wasn't loaded"));        
    }
}
void UIntelRealsenseTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
    Super::TickComponent(DeltaTime, TickType,ThisTickFunction);
    if(successful){
        float* currentPose = m_GetLatestPose(TrackerID);// this pose is a 7 value array, tx, ty, tz, qx, qy, qz, qw
  //      UE_LOG(LogTemp, Warning, TEXT("Translation: %s"), *FVector(currentPose[0],currentPose[1],currentPose[2]).ToString());
//        UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *FQuat(currentPose[3],currentPose[4],currentPose[5],currentPose[6]).ToString());
        
        //UE_LOG(LogTemp, Warning, TEXT("Obtained pose!"));          
        //we need to translate the output coordinate system into an unreal readable format, then apply it to the attached transform
        this->GetOwner()->SetActorLocationAndRotation(FVector(currentPose[2]*100,
        
        currentPose[0]*100,
        currentPose[1]*100
        ),
        FQuat(currentPose[5],
        currentPose[3],
        currentPose[4],
        currentPose[6])
        );        
    }

}
void UIntelRealsenseTracker::EndPlay(const EEndPlayReason::Type EndPlayReason){

    if(successful){
        UE_LOG(LogTemp, Warning, TEXT("Stopping trackers!"));                        
       // trackerInstance = NULL;                                
        m_StopTrackers(TrackerID);
        m_RegisterDeltaPoseUpdate(TrackerID,NULL);
        freeDLL();
        UE_LOG(LogTemp, Warning, TEXT("Free'd the DLL!"));        

    }
    Super::EndPlay(EndPlayReason);    
}
void UIntelRealsenseTracker::CallRenderedFrameCallbackLocally(){
    m_PostRenderReset(TrackerID);
}
void UIntelRealsenseTracker::RenderedFrameCallback(){
    if(intelRealsenseInstance != nullptr){
//        UE_LOG(LogTemp, Warning, TEXT("Renderered a frame"));    
        intelRealsenseInstance->CallRenderedFrameCallbackLocally();
    }
}

void UIntelRealsenseTracker::ApplySettings(UEskyDataContainer* dataContainer){
    for(int i = 0; i < 16; i++){
         leftEyeTransformFromTracker[i] = dataContainer->myConfig.leftEyeTransformFromTracker[i];
         rightEyeTransformFromTracker[i] = dataContainer->myConfig.rightEyeTransformFromTracker[i];         
    }
}
#pragma endregion Unload DLL
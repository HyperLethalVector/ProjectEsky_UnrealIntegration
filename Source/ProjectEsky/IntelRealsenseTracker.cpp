// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectEsky.h"
#include "IntelRealsenseTracker.h"
#pragma region DLL callback function definitions
typedef void(*FuncReceiveCameraImageCallbackWithID)(int instanceID, unsigned char* info, int lengthofarray, int width, int height, int pixelCount);
typedef void(*LocalizationEventCallback)(int trackerID, int LocalizationDelegate);
typedef void(*MapDataCallback)(int trackerID);
typedef void(*LocalizationPoseReceivedCallback)(int trackerID, string ObjectID, float tx, float ty, float tz, float qx, float qy, float qz, float qw);
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
#pragma endregion
void *v_dllHandle;

#pragma region Load DLL

// Method to import a DLL.
bool UIntelRealsenseTracker::importDLL(FString folder, FString name)
{
    FString filePath = *FPaths::GamePluginsDir() + folder + "/" + name;

    if (FPaths::FileExists(filePath))
    {
        v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
        if (v_dllHandle != NULL)
        {
            return true;
        }
    }
    return false;    // Return an error.
}
#pragma endregion Load DLL
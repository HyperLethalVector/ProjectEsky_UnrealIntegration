// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraUtils.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraTypes.h"

FMinimalViewInfo UCameraUtils::GetCameraView(USceneCaptureComponent2D* SceneCapture2D, float DeltaTime)
{
	FMinimalViewInfo view;
	SceneCapture2D->GetCameraView(DeltaTime, view);
	return (view);
}

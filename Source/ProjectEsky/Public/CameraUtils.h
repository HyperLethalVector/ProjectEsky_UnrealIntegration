// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraTypes.h"
#include "CameraUtils.generated.h"

/**
 *
 */
UCLASS()
class UCameraUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CameraUtils")
		static FMinimalViewInfo GetCameraView(USceneCaptureComponent2D* SceneCapture, float DeltaTime);
};

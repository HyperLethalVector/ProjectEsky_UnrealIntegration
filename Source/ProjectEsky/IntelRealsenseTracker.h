// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IntelRealsenseTracker.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=IntelTracker,meta = (BlueprintSpawnableComponent)) 
class UIntelRealsenseTracker : public UActorComponent
{
	GENERATED_BODY()
public: 
	UIntelRealsenseTracker();
	bool successful = false;
	static bool importDLL();

	//LLAPI  Hooks
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static void freeDLL();
	UEskyNativeRenderer* myAttachedRenderer;
	UPROPERTY(EditAnywhere, Category="Intel Tracker Settings")
	int TrackerID;//UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UFUNCTION(BlueprintCallable,Category="Intel Tracker Settings")
	void SetAttachedRenderer(UEskyNativeRenderer* rendererToAttach);
	float* leftEyeTransformFromTracker = new float[16]{1,0,0,0,0,0.9092361,-0.4162807,0,0,0.4162807,0.9092361,0,-0.0284,0.06417969,0.06949941,1};
	float* rightEyeTransformFromTracker = new float[16]{1,0,0,0,0,0.9092361,-0.4162807,0,0,0.4162807,0.9092361,0,0.0356,0.06417969,0.06949941,1};
	static void RenderedFrameCallback();
	void CallRenderedFrameCallbackLocally();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

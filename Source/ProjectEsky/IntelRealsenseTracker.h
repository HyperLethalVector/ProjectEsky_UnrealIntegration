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
	UPROPERTY(EditAnywhere, Category="Intel Tracker Settings")
	int TrackerID;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EskyNativeRenderer.generated.h"
UCLASS( ClassGroup=(ProjectEsky), meta=(BlueprintSpawnableComponent) )
class PROJECTESKY_API UEskyNativeRenderer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEskyNativeRenderer();
	bool successful = false;
	UPROPERTY(EditAnywhere, Category="Esky Renderer Settings")
	int WindowID;
	UPROPERTY(EditAnywhere, Category="Esky Renderer Settings")
	int width; 
	UPROPERTY(EditAnywhere, Category="Esky Renderer Settings")	 
	int height;
	static bool importDLL();
	static void freeDLL();
	void DebugMessage(const char * messageToReturn);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

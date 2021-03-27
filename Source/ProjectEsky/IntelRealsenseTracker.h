// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IntelRealsenseTracker.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESKY_API UIntelRealsenseTracker : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public: 

	//LLAPI  Hooks
	UFUNCTION(BlueprintCallable, Category = "libProjectEskyLLAPIIntel")
	static void OnInitialization();
	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
    static bool importDLL( FString folder, FString name);
	UFUNCTION(BlueprintCallable, Category = "libProjectEskyLLAPIIntel")
	static void freeDLL();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/ActorComponent.h"
#include "EskyDataContainer.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "EskyNativeRenderer.generated.h"


UCLASS( ClassGroup=(ProjectEsky), meta=(BlueprintSpawnableComponent))
class UEskyNativeRenderer : public UActorComponent
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
	UPROPERTY(EditAnywhere, Category="Esky Renderer Settings")
	int xPlacement; 
	UPROPERTY(EditAnywhere, Category="Esky Renderer Settings")	 
	int yPlacement;
        UPROPERTY(EditAnywhere, Category="Esky Renderer Settings")	 
	bool useTemporalReprojection;
        
	float* LeftEyeProjectionMatrix = new float[16]{1.285333,0,0,0,0,1.428148,0,0,0,0,-1.0002,-0.20002,0,0,-1,0};
	float* RightEyeProjectionMatrix = new float[16]{1.285333,0,0,0,0,1.428148,0,0,0,0,-1.0002,-0.20002,0,0,-1,0};
	float* LeftEyeInvProjectionMatrix = new float[16]{0.7780085,0,0,0,0,0.7002077,0,0,0,0,0,-1,0,0,-4.9995,5.005};
	float* RightEyeInvProjectionMatrix = new float[16]{0.7780085,0,0,0,0,0.7002077,0,0,0,0,0,-1,0,0,-4.9995,5.005};
	float* LeftOffset = new float[4]{0.0,0.0};
	float* RightOffset = new float[4]{0.0,0.0};
	float* eyeBorders = new float[8]{0.0,1.0,0.0,1.0,.0,1.0,0.0,1.0};
        
        UIntelRealsenseTracker* myAttachedTracker;
	
	float* LeftUVToRectX = new float[16] {-0.3844103217124939,
        0.9711242318153381,
        -0.2508922219276428,
        0.2063561975955963,
        0.058278605341911319,
        0.13342826068401338,
        0.2762235105037689,
        -0.07580044865608216,
        -0.14823228120803834,
        0.8672905564308167,
        -2.0916364192962648,
        1.2395849227905274,
        0.032221343368291858,
        -0.2913084030151367,
        0.9610205292701721,
        -0.40973442792892458};
	
	float* LeftUVToRectY = new float[16] { -0.3913276493549347,
        0.13756594061851502,
        -0.374609112739563,
        -0.026859400793910028,
        0.9647268056869507,
        -0.5817299485206604,
        1.6475456953048707,
        -1.0525500774383546,
        -0.36617857217788699,
        0.9328171014785767,
        -3.1165359020233156,
        2.242710828781128,
        0.30805620551109316,
        -0.5150173306465149,
        1.8977105617523194,
        -1.4114274978637696};
	
	float* RightUVToRectX = new float[16] {-0.3506585359573364,
        0.9476522207260132,
        0.17013339698314668,
        0.22676172852516175,
        -0.6992087960243225,
        4.4123382568359379,
        -8.646343231201172,
        4.291323661804199,
        1.7034832239151,
        -11.080245018005371,
        20.835912704467775,
        -11.228776931762696,
        -1.1326539516448975,
        7.176768779754639,
        -13.467972755432129,
        7.434933662414551};
		 
	float* RightUVToRectY = new float[16] {-0.31777408719062807,
        -0.01822761259973049,
        -0.030633892863988878,
        0.27896520495414736,
        0.9705286026000977,
        0.10243307054042816,
        0.528424859046936,
        -0.5316285490989685,
        -0.18315571546554566,
        -0.5001878142356873,
        -0.7620742917060852,
        1.1309611797332764,
        0.12612685561180116,
        0.28473961353302,
        0.5733417272567749,
        -0.7395226955413818};
        static bool importDLL();
	static void freeDLL();  
        void SetDeltasLocal(int iD, float* leftEye, float* rightEye);
        static void SetDeltas(int ID, float* leftEye, float* rightEye);
        UFUNCTION(BlueprintCallable,Category="Esky Renderer Settings") 
        void SetAttachedTracker(UIntelRealsenseTracker* trackerToAttach);
        UFUNCTION(BlueprintCallable,Category="Esky Config Settings")
	void ApplySettings(UEskyDataContainer* dataContainer);
        UFUNCTION(BlueprintCallable,Category="Esky Render Functions")        
        void CloseRenderer();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
        
};

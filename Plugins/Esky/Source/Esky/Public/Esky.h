// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Modules/ModuleManager.h"
#include "EskyConfigContainer.h"

DECLARE_LOG_CATEGORY_EXTERN(EskyLog, Log, All);

class FEskyModule : public IModuleInterface
{
public:
    int WindowID = 0;
    int width= 0;
    int height = 0;
    int xPlacement = 0;
    int yPlacement = 0;
    bool useTemporalReprojection = false;
    float* LeftEyeProjectionMatrix = new float[16]{ 1.285333,0,0,0,0,1.428148,0,0,0,0,-1.0002,-0.20002,0,0,-1,0 };
    float* RightEyeProjectionMatrix = new float[16]{ 1.285333,0,0,0,0,1.428148,0,0,0,0,-1.0002,-0.20002,0,0,-1,0 };
    float* LeftEyeInvProjectionMatrix = new float[16]{ 0.7780085,0,0,0,0,0.7002077,0,0,0,0,0,-1,0,0,-4.9995,5.005 };
    float* RightEyeInvProjectionMatrix = new float[16]{ 0.7780085,0,0,0,0,0.7002077,0,0,0,0,0,-1,0,0,-4.9995,5.005 };
    float* LeftOffset = new float[4]{ 0.0,0.0 };
    float* RightOffset = new float[4]{ 0.0,0.0 };
    float* eyeBorders = new float[8]{ 0.0,1.0,0.0,1.0,.0,1.0,0.0,1.0 };

    float* LeftUVToRectX = new float[16]{ -0.3844103217124939,
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
        -0.40973442792892458 };

    float* LeftUVToRectY = new float[16]{ -0.3913276493549347,
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
        -1.4114274978637696 };

    float* RightUVToRectX = new float[16]{ -0.3506585359573364,
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
        7.434933662414551 };

    float* RightUVToRectY = new float[16]{ -0.31777408719062807,
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
        -0.7395226955413818 };
    FConfigInformation myInfo;
    FEskyModule* instance;
    /** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    void LoadConfig(FString fileName);
	void* GetTrackerDLLHandle();
	void* GetRendererDLLHandle();
    void StartRendererComponent();
    void StopRendererComponent();
};

#pragma once
void* TrackerDLLHandlePlugin;
void* RendererDLLHandlePlugin;
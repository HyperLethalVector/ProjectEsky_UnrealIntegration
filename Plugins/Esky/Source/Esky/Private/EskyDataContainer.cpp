// Fill out your copyright notice in the Description page of Project Settings.


#include "EskyDataContainer.h"
UEskyDataContainer* instance;
// Sets default values for this component's properties
UEskyDataContainer::UEskyDataContainer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEskyDataContainer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEskyDataContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
FConfigInfo UEskyDataContainer::LoadConfig(FString fileName){
	FString file = FPaths::ProjectConfigDir();
	file.Append(fileName);	
	FString resultingJson;
	UE_LOG(LogTemp, Warning, TEXT("Loading file: %s"), *file);   	
	FFileHelper::LoadFileToString(resultingJson,*file);
	if(FJsonObjectConverter::JsonObjectStringToUStruct(resultingJson, &myConfig, 0, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Loaded config successfully"));   				
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Didn't load config successfully"));   				
	}
	instance = this;
	return myConfig;
}
void UEskyDataContainer::SaveConfig(FString fileName){
	FString file = FPaths::ProjectConfigDir();
	file.Append(fileName);
	FString outputString;
	UE_LOG(LogTemp, Warning, TEXT("Saving file: %s"), *file);   
	FJsonObjectConverter::UStructToJsonObjectString(myConfig,outputString);
	UE_LOG(LogTemp, Warning, TEXT("Converted Result: %s"), *outputString);   
	FFileHelper::SaveStringToFile(outputString,*file);
}
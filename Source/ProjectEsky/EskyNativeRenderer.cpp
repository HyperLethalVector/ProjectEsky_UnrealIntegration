// Fill out your copyright notice in the Description page of Project Settings.
#include "EskyNativeRenderer.h"

// Sets default values for this component's properties
UEskyNativeRenderer::UEskyNativeRenderer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEskyNativeRenderer::BeginPlay()
{
	Super::BeginPlay();
//	FD3D11DynamicRHI* D3D11RHIPtr = Cast<FD3D11DynamicRHI>(GDynamicRHI);
//	ID3D11Device* DevicePtr = D3D11RHIPtr->GetDevice();
	// ...
	
}


// Called every frame
void UEskyNativeRenderer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


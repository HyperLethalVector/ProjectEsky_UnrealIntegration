#include "ConfigContainer.h"
#include <string>
#include <locale>
ConfigContainer* configInstance;
UConfigContainer::LoadConfig(FString filename){
    UE_LOG(LogTemp, Warning, TEXT("Loading: %s"),filename);
    configInstance = this;
}
UConfigContainer::SaveConfig(FString filename){
    UE_LOG(LogTemp, Warning, TEXT("Saving: %s"),filename);
}
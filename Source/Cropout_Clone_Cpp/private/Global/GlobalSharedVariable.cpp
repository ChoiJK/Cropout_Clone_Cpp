#include "Global/GlobalSharedVariable.h"
#include "Core/CropoutGameInstance.h"
#include "Materials/MaterialParameterCollection.h"

FGlobalSharedVariable::FGlobalSharedVariable(UCropoutGameInstance* gameInstance)
{
	this->GameInstance = gameInstance;
	this->EventDispatcher = GameInstance->GetGlobalEventDispatcher();

	Initialize();
}

void FGlobalSharedVariable::Initialize()
{
	FName path = TEXT("/Game/Blueprint/Misc/BP_SpawnMarker.BP_SpawnMarker_C");
	SpawnMarkerClass = StaticLoadClass(AActor::StaticClass(), nullptr, *path.ToString());
	checkf(SpawnMarkerClass != nullptr, TEXT("Failed to load BP_SpawnMarker"));

	path = TEXT("/Game/Resources/Materials/MPC_Landscape.MPC_Landscape");
	LandScapeMPC = Cast<UMaterialParameterCollection>(
		StaticLoadObject(UMaterialParameterCollection::StaticClass(), nullptr, *path.ToString()));
	checkf(LandScapeMPC != nullptr, TEXT("Failed to load MPC_Landscape"));

	path = TEXT("Material'/Game/Resources/Materials/M_Landscape.M_Landscape'");
	LandScapeMaterial = Cast<UMaterial>(
		StaticLoadObject(UMaterial::StaticClass(), nullptr, *path.ToString()));
	checkf(LandScapeMaterial != nullptr, TEXT("Failed to load M_Landscape"));
}

UClass* FGlobalSharedVariable::GetSpawnMarkerClass()
{
	return SpawnMarkerClass;
}

UMaterialParameterCollection* FGlobalSharedVariable::GetLandScapeMPC()
{
	return LandScapeMPC;
}

UMaterial* FGlobalSharedVariable::GetLandScapeMaterial()
{
	return LandScapeMaterial;
}

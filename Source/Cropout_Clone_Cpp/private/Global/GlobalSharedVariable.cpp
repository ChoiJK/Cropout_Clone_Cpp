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
	SpawnMarkerClass = LoadClass("/Game/Blueprint/Misc/BP_SpawnMarker.BP_SpawnMarker_C");
	LandScapeMPC = LoadObject<UMaterialParameterCollection>("/Game/Resources/Materials/MPC_Landscape.MPC_Landscape");
	LandScapeMaterial = LoadObject<UMaterial>("Material'/Game/Resources/Materials/M_Landscape.M_Landscape'");
	CropoutMPC = LoadObject<UMaterialParameterCollection>(
		"MaterialParameterCollection'/Game/Resources/Materials/MPC_Cropout.MPC_Cropout'");
	PlaceableMaterial = LoadObject<UMaterial>("Material'/Game/Resources/Materials/M_Placeable.M_Placeable'");
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

UMaterialParameterCollection* FGlobalSharedVariable::GetCropoutMPC()
{
	return CropoutMPC;
}

UMaterial* FGlobalSharedVariable::GetPlaceableMaterial()
{
	return PlaceableMaterial;
}

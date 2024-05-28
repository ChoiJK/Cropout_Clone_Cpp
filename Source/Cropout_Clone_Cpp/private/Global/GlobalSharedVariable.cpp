#include "Global/GlobalSharedVariable.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialParameterCollection.h"

UGlobalSharedVariable::UGlobalSharedVariable()
{
	Initialize();
}

void UGlobalSharedVariable::Initialize()
{
	for(int i = 0; i < static_cast<int>(EPrimitiveShapeType::Count); i++)
	{
		PrimitiveShapeMeshs[i] = LoadObject<UStaticMesh>(PrimitiveShapeTypeToPath(static_cast<EPrimitiveShapeType>(i)));
	}

	SpawnMarkerClass = LoadClass<AActor>("/Game/Blueprint/Misc/BP_SpawnMarker.BP_SpawnMarker_C");
	LandScapeMPC = LoadObject<UMaterialParameterCollection>("/Game/Resources/Materials/MPC_Landscape.MPC_Landscape");
	LandScapeMaterial = LoadObject<UMaterial>("Material'/Game/Resources/Materials/M_Landscape.M_Landscape'");
	CropoutMPC = LoadObject<UMaterialParameterCollection>(
		"MaterialParameterCollection'/Game/Resources/Materials/MPC_Cropout.MPC_Cropout'");
	PlaceableMaterial = LoadObject<UMaterial>("Material'/Game/Resources/Materials/M_Placeable.M_Placeable'");
	RT_Draw = LoadObject<UTextureRenderTarget2D>(
		"TextureRenderTarget2D'/Game/Resources/RenderTarget/RT_GrassMove.RT_GrassMove'");
}

void UGlobalSharedVariable::SetCropoutGameInstance(UCropoutGameInstance* instance)
{
	GameInstance = instance;
}

UStaticMesh* UGlobalSharedVariable::GetPrimitiveShapeMesh(EPrimitiveShapeType type)
{
	return PrimitiveShapeMeshs[static_cast<int>(type)];
}

UClass* UGlobalSharedVariable::GetSpawnMarkerClass()
{
	return SpawnMarkerClass;
}

UMaterialParameterCollection* UGlobalSharedVariable::GetLandScapeMPC()
{
	return LandScapeMPC;
}

UMaterial* UGlobalSharedVariable::GetLandScapeMaterial()
{
	return LandScapeMaterial;
}

UMaterialParameterCollection* UGlobalSharedVariable::GetCropoutMPC()
{
	return CropoutMPC;
}

UMaterial* UGlobalSharedVariable::GetPlaceableMaterial()
{
	return PlaceableMaterial;
}

UTextureRenderTarget2D* UGlobalSharedVariable::GetRT_Draw()
{
	return RT_Draw;
}

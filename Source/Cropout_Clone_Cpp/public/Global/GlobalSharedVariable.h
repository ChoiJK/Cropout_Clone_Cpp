#pragma once

class FGlobalEventDispatcher;
class UCropoutGameInstance;

class CROPOUT_CLONE_CPP_API FGlobalSharedVariable
{
public:
	FGlobalSharedVariable(UCropoutGameInstance* gameInstance);

private:
	void Initialize();

	UCropoutGameInstance* GameInstance;
	TWeakPtr<FGlobalEventDispatcher> EventDispatcher;

	UClass* SpawnMarkerClass = nullptr;
	UMaterialParameterCollection* LandScapeMPC = nullptr;
	UMaterial* LandScapeMaterial = nullptr;

public:
	UClass* GetSpawnMarkerClass();
	UMaterialParameterCollection* GetLandScapeMPC();
	UMaterial* GetLandScapeMaterial();

private:
};

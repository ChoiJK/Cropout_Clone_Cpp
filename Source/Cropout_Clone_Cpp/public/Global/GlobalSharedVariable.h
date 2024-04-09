#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Enum/PrimitiveShapeType.h"
#include "GlobalSharedVariable.generated.h"

class FGlobalEventDispatcher;
class UCropoutGameInstance;

UCLASS()
class CROPOUT_CLONE_CPP_API UGlobalSharedVariable : public UObject
{
	GENERATED_BODY()

public:
	UGlobalSharedVariable();

private:
	void Initialize();

	template <typename T>
	T* LoadObject(const FString& path)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");

		static ConstructorHelpers::FObjectFinder<T> loadedObj(*path);
		checkf(loadedObj.Succeeded(), TEXT("Failed to load object: %s"), *path);

		return Cast<T>(loadedObj.Object);
	}

	template <typename T>
	UClass* LoadClass(const FString& path)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");

		static ConstructorHelpers::FClassFinder<T> loadedObj(*path);
		checkf(loadedObj.Succeeded(), TEXT("Failed to load object: %s"), *path);

		UClass* loadedClass = loadedObj.Class;
		checkf(loadedClass != nullptr, TEXT("Failed to load class: %s"), *path);

		return loadedClass;
	}

	UCropoutGameInstance* GameInstance;
	TWeakPtr<FGlobalEventDispatcher> EventDispatcher;

	UStaticMesh* PrimitiveShapeMeshs[static_cast<int>(EPrimitiveShapeType::Count)];

	UClass* SpawnMarkerClass = nullptr;
	UMaterialParameterCollection* LandScapeMPC = nullptr;
	UMaterial* LandScapeMaterial = nullptr;

	UMaterialParameterCollection* CropoutMPC = nullptr;
	UMaterial* PlaceableMaterial = nullptr;

public:
	void SetCropoutGameInstance(UCropoutGameInstance* instance);

	UStaticMesh* GetPrimitiveShapeMesh(EPrimitiveShapeType type);
	UClass* GetSpawnMarkerClass();
	UMaterialParameterCollection* GetLandScapeMPC();
	UMaterial* GetLandScapeMaterial();
	UMaterialParameterCollection* GetCropoutMPC();
	UMaterial* GetPlaceableMaterial();

private:
};

#pragma once

class FGlobalEventDispatcher;
class UCropoutGameInstance;

class CROPOUT_CLONE_CPP_API FGlobalSharedVariable
{
public:
	FGlobalSharedVariable(UCropoutGameInstance* gameInstance);

private:
	void Initialize();

	template <typename T>
	T* LoadObject(const FString& path)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");

		UObject* loadedObj = StaticLoadObject(T::StaticClass(), nullptr, *path);
		checkf(loadedObj != nullptr, TEXT("Failed to load object: %s"), *path);

		return Cast<T>(loadedObj);
	}

	UClass* LoadClass(const FString& path)
	{
		UClass* loadedObj = StaticLoadClass(AActor::StaticClass(), nullptr, *path);
		checkf(loadedObj != nullptr, TEXT("Failed to load class: %s"), *path);

		return loadedObj;
	}

	UCropoutGameInstance* GameInstance;
	TWeakPtr<FGlobalEventDispatcher> EventDispatcher;

	UClass* SpawnMarkerClass = nullptr;
	UMaterialParameterCollection* LandScapeMPC = nullptr;
	UMaterial* LandScapeMaterial = nullptr;

	UMaterialParameterCollection* CropoutMPC = nullptr;
	UMaterial* PlaceableMaterial = nullptr;

public:
	UClass* GetSpawnMarkerClass();
	UMaterialParameterCollection* GetLandScapeMPC();
	UMaterial* GetLandScapeMaterial();
	UMaterialParameterCollection* GetCropoutMPC();
	UMaterial* GetPlaceableMaterial();

private:
};

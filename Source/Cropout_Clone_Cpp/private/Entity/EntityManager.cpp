// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/EntityManager.h"

#include "Entity/Interactable/InteractableBaseActor.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "Entity/Interactable/Resource/ResourceBaseActor.h"
#include "Entity/Interactable/Crop/CropBaseActor.h"
#include "SimpleRx/SimpleRx.h"

void UEntityManager::Init()
{
	Super::Init();
}

void UEntityManager::Prepare()
{
	Super::Prepare();
}

void UEntityManager::Release()
{
	Super::Release();
}

void UEntityManager::Destroy()
{
	Super::Destroy();
}

void UEntityManager::AddInteractable(AInteractableBaseActor* Interactable)
{
	if(Interactables.Contains(Interactable))
	{
		return;
	}

	Interactables.Add(Interactable);
}

void UEntityManager::RemoveInteractable(AInteractableBaseActor* Interactable)
{
	if(Interactables.Contains(Interactable) == false)
	{
		return;
	}

	Interactables.Remove(Interactable);
}

void UEntityManager::AddBuilding(ABuildingBaseActor* Building, bool bRequireBuild)
{
	AddInteractable(Cast<AInteractableBaseActor>(Building));

	if(Buildings.Contains(Building) == false)
	{
		Buildings.Add(Building);
	}

	if(bRequireBuild)
	{
		if(BuildingsRequireBuild.Contains(Building) == false)
		{
			BuildingsRequireBuild.Add(Building);
		}
	}
}

void UEntityManager::RemoveBuilding(ABuildingBaseActor* Building)
{
	RemoveInteractable(Cast<AInteractableBaseActor>(Building));
	if(Buildings.Contains(Building))
	{
		Buildings.Remove(Building);
	}

	if(BuildingsRequireBuild.Contains(Building))
	{
		BuildingsRequireBuild.Remove(Building);
	}
}

void UEntityManager::ChangeBuildState(ABuildingBaseActor* Building, bool bRequireBuild)
{
	if(BuildingsRequireBuild.Contains(Building) && !bRequireBuild)
	{
		BuildingsRequireBuild.Remove(Building);
	}
	else if(!BuildingsRequireBuild.Contains(Building) && bRequireBuild)
	{
		BuildingsRequireBuild.Add(Building);
	}
}

int UEntityManager::GetRequireBuildBuildingCount()
{
	return BuildingsRequireBuild.Num();
}

ABuildingBaseActor* UEntityManager::FindNearestRequireBuildBuilding(FVector Location, int selectionIndex)
{
	if(BuildingsRequireBuild.Num() == 0)
	{
		return nullptr;
	}

	if(BuildingsRequireBuild.Num() <= selectionIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingsRequireBuild.Num() <= selectionIndex"));
		return nullptr;
	}

	const int currentFrameCount = GFrameCounter;

	// 한프레임에 여러번 호출 될 수 있으나 한번만 정렬하도록
	if(LastSortedBuildingFrameCount != currentFrameCount ||
		FVector::Distance(LastSortedBuildingLocation, Location) > 0.1f)
	{
		for(ABuildingBaseActor* Building : BuildingsRequireBuild)
		{
			Building->CalcTemporaryDistanceFromLocation(Location);
		}

		BuildingsRequireBuild.Sort([Location](const ABuildingBaseActor& A, const ABuildingBaseActor& B)
		{
			return A.GetDistSquared() < B.GetDistSquared();
		});

		LastSortedBuildingFrameCount = currentFrameCount;
		LastSortedBuildingLocation = Location;
	}

	return BuildingsRequireBuild[selectionIndex];
}

void UEntityManager::AddResource(AResourceBaseActor* Resource)
{
	AddInteractable(Cast<AInteractableBaseActor>(Resource));
	if(Resources.Contains(Resource) == false)
	{
		Resources.Add(Resource);
	}
	TArray<AResourceBaseActor*>& resourcesByType = ResourcesByType.FindOrAdd(Resource->ResourceType);
	if(resourcesByType.Contains(Resource) == false)
	{
		resourcesByType.Add(Resource);
	}
}

void UEntityManager::RemoveResource(AResourceBaseActor* Resource)
{
	RemoveInteractable(Cast<AInteractableBaseActor>(Resource));
	if(Resources.Contains(Resource))
	{
		Resources.Remove(Resource);
	}

	TArray<AResourceBaseActor*>& resourcesByType = ResourcesByType.FindOrAdd(Resource->ResourceType);
	if(resourcesByType.Contains(Resource))
	{
		resourcesByType.Remove(Resource);
	}
}

int UEntityManager::GetResourceOfTypeCount(EResourceType ResourceType)
{
	return ResourcesByType.FindOrAdd(ResourceType).Num();
}

AResourceBaseActor* UEntityManager::FindNearestResourceOfType(EResourceType ResourceType, FVector Location,
	int selectionIndex)
{
	TArray<AResourceBaseActor*>& ResourcesOfType = ResourcesByType.FindOrAdd(ResourceType);

	if(ResourcesOfType.Num() == 0)
	{
		return nullptr;
	}

	if(ResourcesOfType.Num() <= selectionIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResourcesOfType.Num() <= selectionIndex, ResourceType = %s"),
			*EnumToString(ResourceType));
		return nullptr;
	}

	const int currentFrameCount = GFrameCounter;
	TTuple<int, FVector>& LastSortedResource = LastSortedResourceFrameCount.FindOrAdd(ResourceType);

	// 한프레임에 여러번 호출 될 수 있으나 한번만 정렬하도록
	if(LastSortedResource.Key != currentFrameCount ||
		FVector::Distance(LastSortedResource.Value, Location) > 0.1f)
	{
		for(AResourceBaseActor* Resource : ResourcesOfType)
		{
			Resource->CalcTemporaryDistanceFromLocation(Location);
		}

		ResourcesOfType.Sort([Location](const AResourceBaseActor& A, const AResourceBaseActor& B)
		{
			return A.GetDistSquared() < B.GetDistSquared();
		});

		LastSortedResource.Key = currentFrameCount;
		LastSortedResource.Value = Location;
	}

	return ResourcesOfType[selectionIndex];
}

void UEntityManager::AddCrop(ACropBaseActor* Crop)
{
	AddInteractable(Cast<AInteractableBaseActor>(Crop));

	if(Crops.Contains(Crop) == false)
	{
		Crops.Add(Crop);
	}

	// 초기에는 무조건 Plant가 필요함
	if(CropsRequirePlant.Contains(Crop) == false)
	{
		CropsRequirePlant.Add(Crop);
	}
}

void UEntityManager::RemoveCrop(ACropBaseActor* Crop)
{
	if(Crops.Contains(Crop))
	{
		Crops.Add(Crop);
	}

	if(CropsRequirePlant.Contains(Crop))
	{
		CropsRequirePlant.Add(Crop);
	}

	if(CropsRequireHarvest.Contains(Crop))
	{
		CropsRequireHarvest.Add(Crop);
	}
}

void UEntityManager::ChangeCropState(ACropBaseActor* Crop, bool bRequireHarvest, bool bRequirePlant)
{
	if(bRequireHarvest)
	{
		if(CropsRequireHarvest.Contains(Crop) == false)
		{
			CropsRequireHarvest.Add(Crop);
		}
	}
	else
	{
		if(CropsRequireHarvest.Contains(Crop))
		{
			CropsRequireHarvest.Remove(Crop);
		}
	}

	if(bRequirePlant)
	{
		if(CropsRequirePlant.Contains(Crop) == false)
		{
			CropsRequirePlant.Add(Crop);
		}
	}
	else
	{
		if(CropsRequirePlant.Contains(Crop))
		{
			CropsRequirePlant.Remove(Crop);
		}
	}
}

int UEntityManager::GetRequireHarvestCropCount()
{
	return CropsRequireHarvest.Num();
}

int UEntityManager::GetRequirePlantCropCount()
{
	return CropsRequirePlant.Num();
}

ACropBaseActor* UEntityManager::FindNearestRequireHarvestCrop(FVector Location, int selectionIndex)
{
	if(CropsRequireHarvest.Num() == 0)
	{
		return nullptr;
	}

	if(CropsRequireHarvest.Num() <= selectionIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("CropsRequireHarvest.Num() <= selectionIndex"));
		return nullptr;
	}

	const int currentFrameCount = GFrameCounter;

	// 한프레임에 여러번 호출 될 수 있으나 한번만 정렬하도록
	if(LastSortedRequireHarvestFrameCount != currentFrameCount ||
		FVector::Distance(LastSortedRequireHarvestLocation, Location) > 0.1f)
	{
		for(ACropBaseActor* Crop : CropsRequireHarvest)
		{
			Crop->CalcTemporaryDistanceFromLocation(Location);
		}

		CropsRequireHarvest.Sort([Location](const ACropBaseActor& A, const ACropBaseActor& B)
		{
			return A.GetDistSquared() < B.GetDistSquared();
		});

		LastSortedRequireHarvestFrameCount = currentFrameCount;
		LastSortedRequireHarvestLocation = Location;
	}

	return CropsRequireHarvest[selectionIndex];
}

ACropBaseActor* UEntityManager::FindNearestRequirePlantCrop(FVector Location, int selectionIndex)
{
	if(CropsRequirePlant.Num() == 0)
	{
		return nullptr;
	}

	if(CropsRequirePlant.Num() <= selectionIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("CropsRequirePlant.Num() <= selectionIndex"));
		return nullptr;
	}

	const int currentFrameCount = GFrameCounter;

	// 한프레임에 여러번 호출 될 수 있으나 한번만 정렬하도록
	if(LastSortedRequirePlantFrameCount != currentFrameCount ||
		FVector::Distance(LastSortedRequirePlantLocation, Location) > 0.1f)
	{
		for(ACropBaseActor* Crop : CropsRequirePlant)
		{
			Crop->CalcTemporaryDistanceFromLocation(Location);
		}

		CropsRequirePlant.Sort([Location](const ACropBaseActor& A, const ACropBaseActor& B)
		{
			return A.GetDistSquared() < B.GetDistSquared();
		});

		LastSortedRequirePlantFrameCount = currentFrameCount;
		LastSortedRequirePlantLocation = Location;
	}

	return CropsRequirePlant[selectionIndex];
}

void UEntityManager::AddVillager(AVillager* Villager)
{
	if(Villagers.Contains(Villager) == false)
	{
		Villagers.Add(Villager);
		UpdateVillagerCount();
	}
}

void UEntityManager::RemoveVillager(AVillager* Villager)
{
	if(Villagers.Contains(Villager))
	{
		Villagers.Remove(Villager);
		UpdateVillagerCount();
	}
}

void UEntityManager::UpdateVillagerCount()
{
	if(VillagerCountRp == nullptr)
	{
		VillagerCountRp = std::make_shared<SimpleRxReactiveProperty<int>>(0);
	}

	VillagerCountRp->SetValue(Villagers.Num());
}

std::shared_ptr<SimpleRxReactiveProperty<int>> UEntityManager::GetVillagerCountRp()
{
	return VillagerCountRp;
}

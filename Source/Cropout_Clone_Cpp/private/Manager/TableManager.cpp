// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TableManager.h"

#include "Data/WidgetTableData.h"
#include "Data/BuildableInfo.h"
#include "Data/InteractableInfo.h"
#include "Data/SpawnerInfo.h"


UTableManager::UTableManager()
{
	if(this->GetFlags() & RF_ClassDefaultObject)
	{
		return;
	}

	InitializeWidgetTable();
	InitializeBuildableTable();
	InitializeInteractableTable();
	InitializeSpawnerTable();
}

void UTableManager::Init()
{
	USystemManager::Init();
}

void UTableManager::InitializeWidgetTable()
{
	ConstructorHelpers::FObjectFinder<UDataTable> widgetDataTable
		(TEXT("Engine.DataTable'/Game/Table/WidgetDataTable.WidgetDataTable'"));
	if(widgetDataTable.Succeeded())
	{
		TArray<FName> rowNames = widgetDataTable.Object->GetRowNames();
		for(auto rowName : rowNames)
		{
			FWidgetTableData* widgetData = widgetDataTable.Object->FindRow<FWidgetTableData>(
				rowName, TEXT("WidgetDataTable"));
			if(widgetData && widgetData->WidgetType != EWidgetType::None)
			{
				WidgetTable.Add(widgetData->WidgetType, widgetData->WidgetClass);
			}
		}
	}
}

void UTableManager::InitializeBuildableTable()
{
	ConstructorHelpers::FObjectFinder<UDataTable> buildableInfos
		(TEXT("Engine.DataTable'/Game/Table/BuildableInfoTable.BuildableInfoTable'"));

	if(buildableInfos.Succeeded())
	{
		TArray<FName> rowNames = buildableInfos.Object->GetRowNames();
		for(auto rowName : rowNames)
		{
			FBuildableInfo* buildableInfo = buildableInfos.Object->FindRow<FBuildableInfo>(
				rowName, TEXT("buildableInfo"));
			if(buildableInfo)
			{
				BuildableTable.Add(buildableInfo->Name, buildableInfo);
			}
		}
	}
}

void UTableManager::InitializeInteractableTable()
{
	ConstructorHelpers::FObjectFinder<UDataTable> interactableTable
		(TEXT("Engine.DataTable'/Game/Table/InteractableInfoTable.InteractableInfoTable'"));

	if(interactableTable.Succeeded())
	{
		TArray<FName> rowNames = interactableTable.Object->GetRowNames();
		for(auto rowName : rowNames)
		{
			FInteractableInfo* interactableInfo = interactableTable.Object->FindRow<FInteractableInfo>(
				rowName, TEXT("interactableInfo"));
			if(interactableInfo)
			{
				InteractableTable.Add(interactableInfo->Name, interactableInfo);
			}
		}
	}
}

void UTableManager::InitializeSpawnerTable()
{
	ConstructorHelpers::FObjectFinder<UDataTable> spawnerTable
		(TEXT("Engine.DataTable'/Game/Table/SpawnerInfoTable.SpawnerInfoTable'"));

	if(spawnerTable.Succeeded())
	{
		TArray<FName> rowNames = spawnerTable.Object->GetRowNames();
		for(auto rowName : rowNames)
		{
			FSpawnerInfo* spawnerInfo = spawnerTable.Object->FindRow<FSpawnerInfo>(
				rowName, TEXT("spawnerInfo"));
			if(spawnerInfo)
			{
				SpawnerTable.Add(spawnerInfo->Name, spawnerInfo);
			}
		}
	}
}

void UTableManager::Prepare()
{
	Super::Prepare();
}

void UTableManager::Release()
{
	USystemManager::Release();
}

void UTableManager::Destroy()
{
	Super::Destroy();
}

TSubclassOf<UUserWidget>* UTableManager::GetWidgetClass(EWidgetType type)
{
	return WidgetTable.Find(type);
}

TArray<const FBuildableInfo*> UTableManager::GetBuildableInfos()
{
	TArray<const FBuildableInfo*> buildableInfos;
	for(auto& buildableInfo : BuildableTable)
	{
		buildableInfos.Add(buildableInfo.Value);
	}
	return buildableInfos;
}

TArray<const FInteractableInfo*> UTableManager::GetInteractableInfos()
{
	TArray<const FInteractableInfo*> interactableInfos;
	for(auto& interactableInfo : InteractableTable)
	{
		interactableInfos.Add(interactableInfo.Value);
	}
	return interactableInfos;
}

const FInteractableInfo* UTableManager::GetInteractableInfo(FName name)
{
	return InteractableTable.FindRef(name);
}

TArray<const FSpawnerInfo*> UTableManager::GetSpawnerInfo()
{
	TArray<const FSpawnerInfo*> spawnerInfos;
	for(auto& spawnerInfo : SpawnerTable)
	{
		spawnerInfos.Add(spawnerInfo.Value);
	}
	return spawnerInfos;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TableManager.h"

#include "Data/WidgetTableData.h"
#include "Data/BuildableInfo.h"

UTableManager::UTableManager()
{
	if(this->GetFlags() & RF_ClassDefaultObject)
	{
		return;
	}

	InitializeWidgetTable();
	InitializeBuildableTable();
}

void UTableManager::Init()
{
	USystemManager::Init();
}

void UTableManager::InitializeWidgetTable()
{
	ConstructorHelpers::FObjectFinder<UDataTable> widgetDataTable
		(TEXT("Engine.DataTable'/Game/UI/Table/WidgetDataTable.WidgetDataTable'"));
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
		(TEXT("Engine.DataTable'/Game/Table/BuildableInfos.BuildableInfos'"));

	if(buildableInfos.Succeeded())
	{
		TArray<FName> rowNames = buildableInfos.Object->GetRowNames();
		for(auto rowName : rowNames)
		{
			FBuildableInfo* buildableInfo = buildableInfos.Object->FindRow<FBuildableInfo>(
				rowName, TEXT("buildableInfo"));
			if(buildableInfo && buildableInfo->InteractableType != EInteractableType::None)
			{
				BuildableTable.Add(buildableInfo->Name, buildableInfo);
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

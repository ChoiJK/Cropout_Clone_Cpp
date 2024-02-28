// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/IslandGen.h"

#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshVoxelFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshDeformFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "GenericPlatform/GenericPlatformProperties.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "Core/CropoutGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AIslandGen::BeginPlay()
{
	Super::BeginPlay();

	auto gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	if(gameInstance)
	{
		eventDispatcher = gameInstance->GetGlobalEventDispatcher();
		globalSharedVariable = gameInstance->GetGlobalSharedVariable();
	}

	SetIslandSeed();

	GenerateIslands(true);
}

void AIslandGen::OnConstruction(const FTransform& Transform)
{
	if(PreConstruct == true)
	{
		GenerateIslands(false);
	}
}

void AIslandGen::GenerateIslands(bool spawnMarkers)
{
	const auto dynMesh = GetDynamicMesh();
	if(dynMesh == nullptr)
	{
		return;
	}
	dynMesh->Reset();
	SetActorLocation(FVector::Zero());

	ClearSpawnPoints();

	FirstStep(spawnMarkers);
	SecondStep();
	ThirdStep();
}

void AIslandGen::FirstStep(bool spawnMarkers)
{
	// islands 개수에 맞춰서 랜덤한 위치에 SpawnPoints에 추가
	const auto dynMesh = GetDynamicMesh();
	if(dynMesh == nullptr)
	{
		return;
	}

	for(int i = 0; i <= Islands; i++)
	{
		Radius = Seed.FRandRange(IslandsSize.X, IslandsSize.Y);
		FVector spawnPoint = Seed.GetUnitVector() * (MaxSpawnDistance / 2);
		spawnPoint.Z = 0;
		SpawnPoints.Add(spawnPoint);

		FTransform transform = FTransform(FRotator(0, 0, 0), FVector(spawnPoint.X, spawnPoint.Y, -800));

		FGeometryScriptPrimitiveOptions options;
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(
			dynMesh,
			options,
			transform,
			Radius,
			Radius / 4,
			1300,
			32,
			1);

		if(spawnMarkers)
		{
			if(TSharedPtr<FGlobalSharedVariable> gsv = globalSharedVariable.Pin())
			{
				UClass* spawnMarker = gsv->GetSpawnMarkerClass();
				if(spawnMarker != nullptr)
				{
					GetWorld()->SpawnActor(spawnMarker, &spawnPoint);
				}
			}
		}
	}

	FGeometryScriptPrimitiveOptions options;
	// 콘들을 합치기위한 박스 추가
	FTransform boxTransform = FTransform(FRotator(0, 0, 0), FVector(0, 0, -800));
	float dimension = MaxSpawnDistance + 10000;
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(dynMesh, options, boxTransform, dimension, dimension, 400);

	// Solidify mesh, smooth/tesselate/calc normals
	EPlatformPerformance platformPerformance = GetPlatformPerformance();
	FGeometryScript3DGridParameters gridParams;
	gridParams.SizeMethod = EGeometryScriptGridSizingMethod::GridResolution;
	gridParams.GridResolution = platformPerformance == EPlatformPerformance::LowEnd ? 60 : 50;
	gridParams.GridCellSize = 0.25;

	FGeometryScriptSolidifyOptions solidifyOptions;
	solidifyOptions.GridParameters = gridParams;
	solidifyOptions.WindingThreshold = 0.5;
	solidifyOptions.bSolidAtBoundaries = false;
	solidifyOptions.ExtendBounds = 0.0;
	solidifyOptions.SurfaceSearchSteps = 64;
	solidifyOptions.bThickenShells = false;
	solidifyOptions.ShellThickness = 1.0;

	UGeometryScriptLibrary_MeshVoxelFunctions::ApplyMeshSolidify(dynMesh, solidifyOptions);
	UGeometryScriptLibrary_MeshNormalsFunctions::SetPerVertexNormals(dynMesh);

	FGeometryScriptMeshSelection MeshSelection;
	FGeometryScriptIterativeMeshSmoothingOptions smoothOptions;
	smoothOptions.NumIterations = 6;
	smoothOptions.Alpha = 0.2;
	UGeometryScriptLibrary_MeshDeformFunctions::ApplyIterativeSmoothingToMesh(dynMesh, MeshSelection, smoothOptions);

	FGeometryScriptPNTessellateOptions tessOptions;
	UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyPNTessellation(dynMesh, tessOptions,
	                                                                   platformPerformance ==
	                                                                   EPlatformPerformance::LowEnd
		                                                                   ? 0
		                                                                   : 2);

	// Delete the section of the mesh not needed any more (Underside)
	FGeometryScriptMeshPlaneCutOptions cutOptions;
	cutOptions.UVWorldDimension = 1;
	cutOptions.bFillHoles = false;
	cutOptions.bFillSpans = false;
	cutOptions.bFlipCutSide = false;

	FTransform cutTransform = FTransform(FRotator(180, 0, 0), FVector(0, 0, -390));
	UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshPlaneCut(dynMesh, cutTransform, cutOptions);

	//Flatten the top and Project uvs
	FGeometryScriptMeshPlaneCutOptions topCutOptions;
	FTransform topCutTransform = FTransform();
	UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshPlaneCut(dynMesh, topCutTransform, topCutOptions);

	FTransform projectUVTransform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(100, 100, 100));
	UGeometryScriptLibrary_MeshUVFunctions::SetMeshUVsFromPlanarProjection(
		dynMesh, 0, projectUVTransform, MeshSelection);
}

void AIslandGen::SecondStep()
{
	// Release all computer meshes and move the island slightly to retrigger nav mesh gen
	const auto dynMesh = GetDynamicMesh();
	if(dynMesh == nullptr)
	{
		return;
	}
	ReleaseAllComputeMeshes();
	AddActorWorldOffset(FVector(0, 0, 0.05)); // retrigger nav mesh gen

	if(TSharedPtr<FGlobalEventDispatcher> shared = eventDispatcher.Pin())
	{
		shared->Dispatch(EGlobalEventType::IslandGenComplete);
	}
}

void AIslandGen::ThirdStep()
{
	UMaterialParameterCollection* landScapeMPC = nullptr;
	if(TSharedPtr<FGlobalSharedVariable> gsv = globalSharedVariable.Pin())
	{
		landScapeMPC = gsv->GetLandScapeMPC();
	}

	if(landScapeMPC == nullptr)
	{
		return;
	}

	auto inst = GetWorld()->GetParameterCollectionInstance(landScapeMPC);

	FLinearColor grassColor;
	if(inst->GetVectorParameterValue("GrassColour", grassColor))
	{
		FLinearColor hsvColor = grassColor.LinearRGBToHSV();
		hsvColor.R = 102.999725 + Seed.FRandRange(0, 90);

		inst->SetVectorParameterValue("GrassColour", hsvColor.HSVToLinearRGB());
	}
}

void AIslandGen::SetIslandSeed(int seed)
{
	Seed = FRandomStream(seed);
}

UDynamicMesh* AIslandGen::GetDynamicMesh()
{
	UMaterial* landScapeMaterial = nullptr;
	if(TSharedPtr<FGlobalSharedVariable> gsv = globalSharedVariable.Pin())
	{
		landScapeMaterial = gsv->GetLandScapeMaterial();
	}
	if(DynMesh == nullptr && landScapeMaterial != nullptr)
	{
		const auto dynMeshComp = GetComponentByClass<UDynamicMeshComponent>();
		DynMesh = dynMeshComp->GetDynamicMesh();
		dynMeshComp->SetMaterial(0, landScapeMaterial);
	}

	return DynMesh;
}

EPlatformPerformance AIslandGen::GetPlatformPerformance()
{
	const FString PlatformName = FPlatformProperties::IniPlatformName();
	// PlatformName이 Android, Switch, IOS일때 LowEnd, 그 외에는 HighEnd
	if(PlatformName == TEXT("Android") || PlatformName == TEXT("Switch") || PlatformName == TEXT("IOS"))
	{
		return EPlatformPerformance::LowEnd;
	}

	return EPlatformPerformance::HighEnd;
}

void AIslandGen::ClearSpawnPoints()
{
	SpawnPoints.Empty();
}

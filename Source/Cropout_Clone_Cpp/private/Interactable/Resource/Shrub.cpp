// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Resource/Shrub.h"


// Sets default values
AShrub::AShrub()
{
	UE_LOG(LogTemp, Warning, TEXT("AShrub Constructor"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshList.Add(
		ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Environment/Meshes/Crops/SM_Shrub_01.SM_Shrub_01'")).Object);
	MeshList.Add(
		ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Environment/Meshes/Crops/SM_Shrub_02.SM_Shrub_02'")).Object);

	ResourceType = EResourceType::Food;
	ResourceAmount = 100;
	CollectionTime = 3.0f;
	CollectionValue = 10;
	UseRandomMesh = true;
	BoundGap = -0.25;
}

// Called when the game starts or when spawned
void AShrub::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShrub::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("AShrub Tick"));
	Super::Tick(DeltaTime);
}

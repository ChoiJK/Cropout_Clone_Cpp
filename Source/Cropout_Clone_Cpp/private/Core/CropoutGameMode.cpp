// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameMode.h"
#include "Core/CropoutGameInstance.h"
#include "Global/GlobalEventDispatcher.h"

void ACropoutGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto gameInstance = Cast<UCropoutGameInstance>( GetGameInstance() );
	if( gameInstance )
	{
		eventDispatcher = gameInstance->GetGlobalEventDispatcher();
	}

	if( eventDispatcher.IsValid() )
	{
		islandGenCompleteHandle = eventDispatcher->AddListenerUObject<ACropoutGameMode>( EGlobalEventType::IslandGenComplete, this, &ACropoutGameMode::OnIslandGenComplete );
		if( islandGenCompleteHandle.IsValid() == false )
		{
			UE_LOG( LogTemp, Error, TEXT( "Failed to add listener for IslandGenComplete" ) );
		}
	}
}

void ACropoutGameMode::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	if( eventDispatcher.IsValid() )
	{
		eventDispatcher->RemoveListener( EGlobalEventType::IslandGenComplete, islandGenCompleteHandle );
	}

	Super::EndPlay( EndPlayReason );
}

void ACropoutGameMode::OnIslandGenComplete()
{
	UE_LOG( LogTemp, Warning, TEXT( "IslandGenComplete" ) );
}

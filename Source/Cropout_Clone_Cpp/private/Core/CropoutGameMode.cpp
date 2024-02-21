// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/CropoutGameMode.h"

#include "Global/GlobalEventDispatcher.h"
#include "Core/CropoutGameInstance.h"

void ACropoutGameMode::StartPlay()
{

	auto gameInstance = Cast<UCropoutGameInstance>( GetGameInstance() );
	if( gameInstance )
	{
		eventDispatcher = gameInstance->GetGlobalEventDispatcher();
	}

	if( TSharedPtr<GlobalEventDispatcher> shared = eventDispatcher.Pin() )
	{
		islandGenCompleteHandle = shared->AddListenerUObject( EGlobalEventType::IslandGenComplete, this, &ACropoutGameMode::OnIslandGenComplete );
		if( islandGenCompleteHandle.IsValid() == false )
		{
			UE_LOG( LogTemp, Error, TEXT( "Failed to add listener for IslandGenComplete" ) );
		}
	}

	Super::StartPlay();
}

void ACropoutGameMode::BeginPlay()
{
	Super::BeginPlay();

	// @ TODO : ScreenEffect FadeOut



}

void ACropoutGameMode::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	if( TSharedPtr<GlobalEventDispatcher> shared = eventDispatcher.Pin() )
	{
		shared->RemoveListener( EGlobalEventType::IslandGenComplete, islandGenCompleteHandle );
	}

	Super::EndPlay( EndPlayReason );
}

void ACropoutGameMode::OnIslandGenComplete()
{
	UE_LOG( LogTemp, Warning, TEXT( "IslandGenComplete" ) );
}

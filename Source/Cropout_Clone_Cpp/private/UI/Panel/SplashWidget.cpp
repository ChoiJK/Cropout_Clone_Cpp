// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/SplashWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Core/CropoutGameInstance.h"
#include "ApplicationState/ApplicationStateManager.h"

void USplashWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SequencePlayer = PlayAnimation(SplashAlphaFadeOut);
	if(SequencePlayer)
	{
		SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &USplashWidget::OnSplashAlphaFadeOutFinished);
	}
}

void USplashWidget::NativeDestruct()
{
	Super::NativeDestruct();
	SequencePlayer->OnSequenceFinishedPlaying().Clear();
}

void USplashWidget::OnSplashAlphaFadeOutFinished(UUMGSequencePlayer& sequencePlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("USplashWidget::OnSplashAlphaFadeOutFinished"));

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::SplashComplete);
}

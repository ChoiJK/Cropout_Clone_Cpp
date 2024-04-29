// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/InGameLayerWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/Element/ResourceWidget.h"
#include "Enum/ResourceType.h"
#include "UI/Element/ButtonWidget.h"
#include "UI/UiManager.h"
#include "CommonButtonBase.h"
#include "Core/CropoutGameMode.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "Entity/EntityManager.h"

#include "SimpleRx/SimpleRx.h"

void UInGameLayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ResourceContainer);

	CompositeDisposable = std::make_shared<SimpleRxCompositeDisposable>();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	TableManager = gameInstance->GetSystemManager<UTableManager>();
	UiManager = gameInstance->GetLevelManager<UUiManager>();

	UEntityManager* entityManager = gameInstance->GetLevelManager<UEntityManager>();
	entityManager->GetVillagerCountRp()->SubscribeAndNotify(std::bind(&UInGameLayerWidget::SetVillagerCount, this,
		std::placeholders::_1))->AddTo(CompositeDisposable);

	TSubclassOf<UUserWidget>* resourceWidget = TableManager->GetWidgetClass(EWidgetType::ResourceElement);

	if(resourceWidget)
	{
		for(int i = 0; i < 3; ++i)
		{
			ResourceWidget[i] = CreateWidget<UResourceWidget>(this, resourceWidget->Get());
			ResourceContainer->AddChildToVerticalBox(ResourceWidget[i]);
			ResourceWidget[i]->SetResourceType(static_cast<EResourceType>(i + 1));
			ResourceWidget[i]->SetValue(0);

			gameInstance->GetCurrentGameMode()->GetResourceRP(static_cast<EResourceType>(i + 1))->
			              SubscribeAndNotify([this, i](int value)
			              {
				              if(ResourceWidget[i])
				              {
					              ResourceWidget[i]->SetValue(value);
				              }
			              })->AddTo(CompositeDisposable);
		}
	}

	// @Todo : GamePad상태일 때 PauseButton을 hide한다.
	//if(IsValid(PauseButton))
	//{
	//	UGameplayStatics::GetPlayerController(GetWorld(                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ), 0)->bind
	//}

	PauseButton->OnClicked().AddUObject(this, &UInGameLayerWidget::OnPauseButtonClicked);
}

void UInGameLayerWidget::NativeDestruct()
{
	Super::NativeDestruct();

	PauseButton->OnClicked().Clear();

	if(CompositeDisposable)
	{
		CompositeDisposable->Dispose();
		CompositeDisposable.reset();
	}
}

void UInGameLayerWidget::OnPauseButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::PauseGame);
}

TOptional<FUIInputConfig> UInGameLayerWidget::GetDesiredInputConfig() const
{
	FUIInputConfig config = FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);

	return config;
}

UResourceWidget* UInGameLayerWidget::GetResourceWidget(int index) const
{
	if(index >= 0 && index < 3)
	{
		return ResourceWidget[index];
	}

	return nullptr;
}

void UInGameLayerWidget::SetVillagerCount(int count)
{
	if(IsValid(VillagerCountText))
	{
		VillagerCountText->SetText(FText::FromString(FString::FromInt(count)));
	}
}

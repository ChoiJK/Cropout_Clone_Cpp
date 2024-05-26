#include "StateProcessSharedData.h"

void UStateProcessSharedData::SetCurrentActivatableWidget(UCommonActivatableWidget* commonActivatableWidget)
{
	CurrentActivatableWidget = commonActivatableWidget;
}

UCommonActivatableWidget* UStateProcessSharedData::GetCurrentActivatableWidget()
{
	return CurrentActivatableWidget;
}

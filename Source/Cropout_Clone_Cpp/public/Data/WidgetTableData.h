#pragma once

#include "CoreMinimal.h"
#include "Enum/WidgetType.h"
#include "WidgetTableData.generated.h"

USTRUCT(BlueprintType)
struct FWidgetTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EWidgetType WidgetType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;
};

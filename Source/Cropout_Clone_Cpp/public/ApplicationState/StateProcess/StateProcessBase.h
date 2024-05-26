#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ApplicationState/StateProcessSharedData.h"
#include "StateProcessBase.generated.h"

UCLASS(Abstract)
class CROPOUT_CLONE_CPP_API UStateProcessBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init() PURE_VIRTUAL(UStateProcessBase::Init,);
	virtual void Process(UStateProcessSharedData* sharedData) PURE_VIRTUAL(UStateProcessBase::Process,);
	virtual bool IsSuccess() PURE_VIRTUAL(UStateProcessBase::IsSuccess, {return false;});
};

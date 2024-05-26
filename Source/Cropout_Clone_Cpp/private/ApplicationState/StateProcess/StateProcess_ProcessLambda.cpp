// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_ProcessLambda.h"

void UStateProcess_ProcessLambda::Init()
{
}

void UStateProcess_ProcessLambda::Process(UStateProcessSharedData* sharedData)
{
	Lambda();
}

bool UStateProcess_ProcessLambda::IsSuccess()
{
	return true;
}

void UStateProcess_ProcessLambda::SetLambda(const TFunction<void()>& InLambda)
{
	Lambda = InLambda;
}

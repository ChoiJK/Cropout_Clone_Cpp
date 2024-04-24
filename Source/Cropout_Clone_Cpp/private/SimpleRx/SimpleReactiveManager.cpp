// Fill out your copyright notice in the Description page of Project Settings.


#include "SImpleRx/SimpleReactiveManager.h"

USimpleReactiveManager* USimpleReactiveManager::Instance = nullptr;

void USimpleReactiveManager::Init()
{
	Super::Init();

	Instance = this;
}

void USimpleReactiveManager::Prepare()
{
	Super::Prepare();
}

void USimpleReactiveManager::Release()
{
	Super::Release();
}

void USimpleReactiveManager::Destroy()
{
	Super::Destroy();
}

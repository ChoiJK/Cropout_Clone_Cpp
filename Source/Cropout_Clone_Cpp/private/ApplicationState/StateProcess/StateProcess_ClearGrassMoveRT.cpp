// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_ClearGrassMoveRT.h"

#include "Core/CropoutGameInstance.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"

void UStateProcess_ClearGrassMoveRT::Init()
{
}

void UStateProcess_ClearGrassMoveRT::Process(UStateProcessSharedData* sharedData)
{
	auto rt_Draw = UCropoutGameInstance::GetInstance()->GetGlobalSharedVariable()->GetRT_Draw();

	if(rt_Draw)
	{
		// ENQUEUE_RENDER_COMMAND를 사용하여 렌더링 스레드에서 실행되는 커맨드를 생성
		ENQUEUE_RENDER_COMMAND(ClearRenderTargetCommand)(
			[RenderTargetResource = rt_Draw->GameThread_GetRenderTargetResource()](FRHICommandListImmediate& RHICmdList)
			{
				// 렌더 타겟을 클리어
				FRHIRenderPassInfo RPInfo(RenderTargetResource->GetRenderTargetTexture(),
					ERenderTargetActions::Clear_Store);
				RHICmdList.BeginRenderPass(RPInfo, TEXT("ClearRenderTarget"));
				RHICmdList.EndRenderPass();
			}
		);

		UE_LOG(LogTemp, Log, TEXT("RenderTarget cleared."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RenderTarget is not set!"));
	}
}


bool UStateProcess_ClearGrassMoveRT::IsSuccess()
{
	return true;
}

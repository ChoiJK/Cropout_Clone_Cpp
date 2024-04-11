#include "Util/CoordinateUtils.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutGameMode.h"
#include "Core/CropoutPlayerController.h"
#include "Kismet/GameplayStatics.h"


ACropoutPlayerController* CoordinateUtils::PlayerController = nullptr;



ACropoutPlayerController* CoordinateUtils::GetPlayerController()
{	
	return PlayerController;
}

FVector2D CoordinateUtils::GetViewportCenter()
{
	auto playerController = GetPlayerController();

	int32 viewportSizeX, viewportSizeY;
	playerController->GetViewportSize(viewportSizeX, viewportSizeY);

	FVector2D screenPos;
	screenPos.X = viewportSizeX / 2.f;
	screenPos.Y = viewportSizeY / 2.f;

	return screenPos;
}

void CoordinateUtils::ProjectViewportCenterToGroundPlane(FVector& IntersectionPos)
{
	auto playerController = GetPlayerController();

	FVector2D screenPos = GetViewportCenter();

	FVector worldLocation, worldDirection;
	UWorld* world = UCropoutGameInstance::GetInstance()->GetWorld();
	UGameplayStatics::GetPlayerController(world, 0)->DeprojectScreenPositionToWorld(
		screenPos.X, screenPos.Y, worldLocation, worldDirection);
	FVector end = worldLocation + (worldDirection * 100000.0f);

	FVector intersectionLoc =
		FMath::LinePlaneIntersection(worldLocation, end, FVector(0, 0, 0), FVector(0, 0, 1));

	IntersectionPos = intersectionLoc;
}


bool CoordinateUtils::ProjectTouchToGroundPlane(FVector2D& ScreenPos, FVector& IntersectionPos)
{
	auto playerController = GetPlayerController();

	FVector2D screenPos = GetViewportCenter();

	bool hasTouch = false;
	float worldLocZ = 0.f;
	switch(playerController->GetCurrentInputType())
	{
	case EInputType::KeyMouse:
		double mouseX, mouseY;
		if(playerController->GetMousePosition(mouseX, mouseY))
		{
			hasTouch = true;
			screenPos.X = mouseX;
			screenPos.Y = mouseY;
		}
		break;
	case EInputType::GamePad:
		hasTouch = true;
	case EInputType::Touch:
		double touchX, touchY;
		bool isCurrentlyTouching;
		playerController->GetInputTouchState(ETouchIndex::Touch1, touchX, touchY, isCurrentlyTouching);
		if(isCurrentlyTouching)
		{
			hasTouch = true;
			screenPos.X = touchX;
			screenPos.Y = touchY;
		}
		else
		{
			worldLocZ = -500.f;
		}
		break;
	}

	FVector worldLocation, worldDirection;
	UWorld* world = UCropoutGameInstance::GetInstance()->GetWorld();
	UGameplayStatics::GetPlayerController(world, 0)->DeprojectScreenPositionToWorld(
		screenPos.X, screenPos.Y, worldLocation, worldDirection);
	FVector end = worldLocation + (worldDirection * 100000.0f);

	FVector intersectionLoc =
		FMath::LinePlaneIntersection(worldLocation, end, FVector(0, 0, 0), FVector(0, 0, 1));

	ScreenPos = screenPos;
	intersectionLoc.Z += worldLocZ;
	IntersectionPos = intersectionLoc;
	return hasTouch;
}

#include "Util/CoordinateUtils.h"
#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGameGameMode.h"
#include "Core/InGame/InGamePlayerController.h"
#include "Kismet/GameplayStatics.h"


AInGamePlayerController* CoordinateUtils::PlayerController = nullptr;


AInGamePlayerController* CoordinateUtils::GetPlayerController()
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

bool CoordinateUtils::ProjectScreenPosToGroundPlane(FVector2D ScreenPos, FVector& IntersectionPos)
{
	auto playerController = GetPlayerController();

	FVector worldLocation, worldDirection;
	UWorld* world = UCropoutGameInstance::GetInstance()->GetWorld();
	UGameplayStatics::GetPlayerController(world, 0)->DeprojectScreenPositionToWorld(
		ScreenPos.X, ScreenPos.Y, worldLocation, worldDirection);
	FVector end = worldLocation + (worldDirection * 100000.0f);

	FVector intersectionLoc =
		FMath::LinePlaneIntersection(worldLocation, end, FVector(0, 0, 0), FVector(0, 0, 1));

	IntersectionPos = intersectionLoc;
	return true;
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

bool CoordinateUtils::SingleTouchCheck()
{
	float locX, locY;
	bool isCurrentlyTouching = false;
	GetPlayerController()->GetInputTouchState(ETouchIndex::Touch2, locX, locY, isCurrentlyTouching);

	return !isCurrentlyTouching;
}

FVector CoordinateUtils::SteppedPosition(const FVector& position)
{
	FVector retVal = position / 200.f;
	retVal.X = FMath::RoundToFloat(retVal.X) * 200.f;
	retVal.Y = FMath::RoundToFloat(retVal.Y) * 200.f;
	retVal.Z = 0.f;

	return retVal;
}

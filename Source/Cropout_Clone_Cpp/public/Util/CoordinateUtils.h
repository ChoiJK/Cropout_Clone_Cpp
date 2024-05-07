#pragma once

#include "CoreMinimal.h"

class AInGamePlayerController;

static class CoordinateUtils
{
private:
	static AInGamePlayerController* PlayerController;
	static AInGamePlayerController* GetPlayerController();

public:
	static void SetPlayerController(AInGamePlayerController* controller)
	{
		PlayerController = controller;
	}

	static FVector2D GetViewportCenter();
	static void ProjectViewportCenterToGroundPlane(FVector& IntersectionPos);
	static bool ProjectScreenPosToGroundPlane(FVector2D ScreenPos, FVector& IntersectionPos);
	static bool ProjectTouchToGroundPlane(FVector2D& ScreenPos, FVector& IntersectionPos);
	static bool SingleTouchCheck();
	static FVector SteppedPosition(const FVector& position);
};

#pragma once

#include "CoreMinimal.h"

class ACropoutPlayerController;

static class CoordinateUtils
{
private:
	static ACropoutPlayerController* PlayerController;
	static ACropoutPlayerController* GetPlayerController();

public:
	static void SetPlayerController(ACropoutPlayerController* controller)
	{
		PlayerController = controller;
	}

	static FVector2D GetViewportCenter();
	static void ProjectViewportCenterToGroundPlane(FVector& IntersectionPos);
	static bool ProjectTouchToGroundPlane(FVector2D& ScreenPos, FVector& IntersectionPos);
	static bool SingleTouchCheck();
	static FVector SteppedPosition(const FVector& position);
};

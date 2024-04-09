#pragma once

#include "CoreMinimal.h"

class ACropoutPlayerController;

static class CoordinateUtils
{
private:
	static ACropoutPlayerController* PlayerController;
	static ACropoutPlayerController* GetPlayerController();

public:
	static FVector2D GetViewportCenter();
	static bool ProjectTouchToGroundPlane(FVector2D& ScreenPos, FVector& IntersectionPos);
};

#pragma once

UENUM()
enum class EVillagerJobType : uint8
{
	None UMETA(DisplayName = "None"),
	Idle UMETA(DisplayName = "Idle"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	Farming UMETA(DisplayName = "Farming"),
	Build UMETA(DisplayName = "Build")
};

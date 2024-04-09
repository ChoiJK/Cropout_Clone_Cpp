#pragma once

#include "CoreMinimal.h"
#include "PrimitiveShapeType.generated.h"

UENUM()
enum class EPrimitiveShapeType : uint8
{
	Cube UMETA(DisplayName = "Cube"),
	Plane UMETA(DisplayName = "Plane"),
	Cylinder UMETA(DisplayName = "Cylinder"),
	Sphere UMETA(DisplayName = "Sphere"),
	Cone UMETA(DisplayName = "Cone"),
	Count UMETA(DisplayName = "Count"),
};

inline FString PrimitiveShapeTypeToPath(EPrimitiveShapeType type)
{
	switch(type)
	{
	case EPrimitiveShapeType::Cube:
		return TEXT("Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	case EPrimitiveShapeType::Plane:
		return TEXT("Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'");
	case EPrimitiveShapeType::Cylinder:
		return TEXT("Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'");
	case EPrimitiveShapeType::Sphere:
		return TEXT("Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	case EPrimitiveShapeType::Cone:
		return TEXT("Engine.StaticMesh'/Engine/BasicShapes/Cone.Cone'");
	default:
		return TEXT("");
	}
}

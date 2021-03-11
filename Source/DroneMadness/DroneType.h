#pragma once
#include "DroneType.generated.h"

UENUM(BlueprintType)
/// <summary>
/// Type of the Drone
/// </summary>
enum EDroneType
{
	DT_Light		UMETA(DisplayName = "Light"),
	DT_Heavy		UMETA(DisplayName = "Heavy")
};


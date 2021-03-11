#pragma once

#include "DroneOrder.generated.h"

USTRUCT(BlueprintType)
/// <summary>
/// Drone order defined by it's direction and distance. 
/// Used to determine drone's movement.
/// </summary>
struct FDroneOrder
{
	GENERATED_BODY()
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector Direction;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Distance;
	
	FDroneOrder();
	
	FDroneOrder(FVector Dir, float Dist);
	
};

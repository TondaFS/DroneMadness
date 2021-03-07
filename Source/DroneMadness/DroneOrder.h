#pragma once

#include "DroneOrder.generated.h"

USTRUCT(BlueprintType)
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

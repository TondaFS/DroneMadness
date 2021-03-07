#include "OrderGeneration.h"
#include "DroneOrder.h"

FDroneOrder FOrderGeneration::GenerateOrder(float MinDist, float MaxDist)
{
	float RandomX = FMath::RandRange(-1.0f, 1.0f);
	float RandomY = FMath::RandRange(-1.0f, 1.0f);
	FVector RandomDirection = FVector(RandomX, RandomY, 0);
	RandomDirection.Normalize();
	float RandomDistance = FMath::RandRange(MinDist, MaxDist);

	return FDroneOrder(RandomDirection, RandomDistance);
}


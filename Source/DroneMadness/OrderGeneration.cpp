#include "OrderGeneration.h"
#include "DroneOrder.h"

/// <summary>
/// Function generates random order with random direction and random distance between range of min/max parameters.
/// </summary>
/// <param name="MinDist">Min destance for the order</param>
/// <param name="MaxDist">Max distance for the order</param>
/// <returns>New order with random direction and distance</returns>
FDroneOrder FOrderGeneration::GenerateOrder(float MinDist, float MaxDist)
{
	float RandomX = FMath::RandRange(-1.0f, 1.0f);
	float RandomY = FMath::RandRange(-1.0f, 1.0f);
	FVector RandomDirection = FVector(RandomX, RandomY, 0);
	RandomDirection.Normalize();
	float RandomDistance = FMath::RandRange(MinDist, MaxDist);

	return FDroneOrder(RandomDirection, RandomDistance);
}


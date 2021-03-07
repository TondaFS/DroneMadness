#pragma once

#include "DroneOrder.h"

class  FOrderGeneration
{
public:
	static FDroneOrder GenerateOrder(float MinDist, float MaxDist);
};

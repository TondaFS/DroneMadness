// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DroneType.generated.h"

UENUM(BlueprintType)
enum EDroneType
{
	DT_Light		UMETA(DisplayName = "Light"),
	DT_Heavy		UMETA(DisplayName = "Heavy")
};


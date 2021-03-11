// Fill out your copyright notice in the Description page of Project Settings.

#include "Drone.h"
#include "DroneType.h"
#include "CommandCenter.h"
#include "OrderGeneration.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();	
	CurrentSpeed = 0;
}

/// <summary>
/// when drone leaves play, unregister from current command center
/// </summary>
/// <param name="EndPlayReason"></param>
void ADrone::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (CurrentCommandCenter != nullptr)
		CurrentCommandCenter->UnregisterDrone(this);
}

//ticks every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	if (IsHit) 
	{
		if (CurrentSpeed > 0) 
		{
			CurrentSpeed -= DeltaTime * Speed;
			if (CurrentSpeed < 0)
				CurrentSpeed = 0;
		}

		return;
	}
				
	MoveDrone(DeltaTime);
}

/// <summary>
/// Moves drone closer to the current destination.
/// </summary>
/// <param name="DeltaTime"></param>
void ADrone::MoveDrone(float DeltaTime) 
{
	//get current movement direction
	FVector CurrentLocation = this->GetActorLocation();
	FVector TargetDirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, MovementDestination);
	FVector Movement = FVector(CurrentLocation + FVector(TargetDirection * DeltaTime * CurrentSpeed));

	//get the distance from destination
	float CurrentDistance = FVector::Distance(Movement, MovementDestination);

	//if we are really close, wait for new order
	if (CurrentDistance <= MovementCloseCheckDistance)
	{
		this->SetActorLocation(MovementDestination);
		CurrentSpeed = 0;

		//drone is not in range of any command center, generate own order
		if (CurrentCommandCenter == nullptr)
			GenerateNewOrder();
	}
	else //drone is not close to the destination, move closer	
	{
		this->SetActorLocation(Movement);

		// update drone speed
		if (CurrentDistance <= (Speed / 2))
		{
			CurrentSpeed -= DeltaTime * Speed;
		}
		else
		{
			if (CurrentSpeed < Speed)
			{
				CurrentSpeed += DeltaTime * Speed;
			}
			else
			{
				CurrentSpeed = Speed;
			}
		}
	}
}

/// <summary>
/// Generates new order based on drones generation variables and immediately sets this order as the current one.
/// </summary>
void ADrone::GenerateNewOrder()
{
	FDroneOrder NewOrder = FOrderGeneration::GenerateOrder(MinDistance, MaxDistance);
	SetNewOrder(NewOrder);
}

/// <summary>
/// Sets given order as the current one. Calculates movement destinatin of this order for the drone.
/// </summary>
/// <param name="NewOrder"></param>
void ADrone::SetNewOrder(FDroneOrder NewOrder)
{
	//Heavy drone only: ignore new order if we were already hit
	if (DroneType == DT_Heavy && IsHit)
		return;

	CurrentOrder = NewOrder;
	MovementDestination = this->GetActorLocation() + NewOrder.Direction * CurrentOrder.Distance;
}

/// <summary>
/// Based on it's type and its damage, drone is immediately destroyed or stopped 
/// </summary>
void ADrone::OnHit()
{	
	//destroy light drone immediately
	if (DroneType == DT_Light) 
	{		
		Destroy();
	}
	else 
	{
		//only if heavy drone was hit, destroy him
		if (IsHit) 
		{
			Destroy();
		}			
		else
		{
			//mark hit and stop the drone
			IsHit = true;
		}
	}	
}

/// <summary>
/// Initiates drone's values.
/// </summary>
/// <param name="Type">Type of the drone</param>
/// <param name="Order">First order</param>
void ADrone::Init(EDroneType Type, FDroneOrder Order)
{
	IsHit = false;
	DroneType = Type;	
	SetNewOrder(Order);
	OnDroneInitiated.Broadcast();
}

/// <summary>
/// Unregisters from the current command center if there is one
/// </summary>
void ADrone::UnregisterFromCurrentCommandCenter()
{
	//unregister from previous command center
	if (CurrentCommandCenter != nullptr)
	{
		CurrentCommandCenter->UnregisterDrone(this);
	}
}

/// <summary>
/// Sets new command center
/// </summary>
/// <param name="CommandCenter"></param>
void ADrone::SetCommandCenter(ACommandCenter* CommandCenter)
{
	CurrentCommandCenter = CommandCenter;
	OnNewCommandCenterRegistered.Broadcast(CurrentCommandCenter != nullptr);
}


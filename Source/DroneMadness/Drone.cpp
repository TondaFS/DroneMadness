// Fill out your copyright notice in the Description page of Project Settings.

#include "Drone.h"
#include "DroneType.h"
#include "CommandCenter.h"
#include "OrderGeneration.h"

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

	GenerateNewOrder();
}

void ADrone::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("Drone is about to be destoryed"));
	if (CurrentCommandCenter != nullptr)
		CurrentCommandCenter->UnregisterDrone(this);
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	if (IsHit)
		return;

	//moves drone in the current direction
	FVector currentLocation = this->GetActorLocation();
	FVector movement = currentLocation + 
		FVector(CurrentOrder.Direction * DeltaTime * Speed);

	float CurrentDistance = FVector::Distance(movement, MovementDestination);

	if (CurrentDistance <= MovementCloseCheckDistance)
	{
		this->SetActorLocation(MovementDestination);
		GenerateNewOrder();
	}
	else 
	{
		this->SetActorLocation(movement);
	}
}

void ADrone::GenerateNewOrder()
{
	FDroneOrder NewOrder = FOrderGeneration::GenerateOrder(MinDistance, MaxDistance);
	SetNewOrder(NewOrder);
}

void ADrone::SetNewOrder(FDroneOrder NewOrder)
{
	//Heavy drone only: ignore new order if we were already hit
	if (DroneType == DT_Heavy && IsHit)
		return;

	CurrentOrder = NewOrder;
	MovementDestination = this->GetActorLocation() + NewOrder.Direction * CurrentOrder.Distance;
}

void ADrone::OnHit()
{	
	//destroy light drone immediately
	if (DroneType == DT_Light) 
	{		
		UE_LOG(LogTemp, Warning, TEXT("Destroying light drone"));
		Destroy();
	}
	else 
	{
		//only if heavy drone was hit, destroy him
		if (IsHit) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroying Heavy Drone"));
			Destroy();
		}			
		else
		{
			//mark hit and stop the drone
			UE_LOG(LogTemp, Warning, TEXT("Stopping Heavy Drone"));
			IsHit = true;
			//SetNewOrder(FDroneOrder(FVector::ZeroVector, 0));
		}
	}	
}

void ADrone::Init(EDroneType Type, FDroneOrder Order)
{
	IsHit = false;
	DroneType = Type;	
	SetNewOrder(Order);
	UE_LOG(LogTemp, Warning, TEXT("Drone init done"));
}

void ADrone::UnregisterFromCurrentCommandCenter()
{
	//unregister from previous command center
	if (CurrentCommandCenter != nullptr)
	{
		CurrentCommandCenter->UnregisterDrone(this);
	}
}

void ADrone::SetCommandCenter(ACommandCenter* CommandCenter)
{
	CurrentCommandCenter = CommandCenter;
	
	//play particle here
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandCenter.h"
#include "Drone.h"
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include "OrderGeneration.h"

// Sets default values
ACommandCenter::ACommandCenter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACommandCenter::BeginPlay()
{
	Super::BeginPlay();

	//get reference for sphere component
	TriggerComponent = FindComponentByClass<USphereComponent>();
	TriggerComponent->SetSphereRadius(ControlRange);

	//set timers for spawning and broadcasting
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ACommandCenter::SpawnDrones, SpawnRate, true, 1.5f);
	GetWorld()->GetTimerManager().SetTimer(OrdersHandle, this, &ACommandCenter::GiveOrders, BroadcastRate, true);
}

/// <summary>
/// When Drone enters trigger, register it to this command center
/// </summary>
/// <param name="Other"></param>
void ACommandCenter::NotifyActorBeginOverlap(AActor* Other)
{
	if (ADrone* Drone = Cast<ADrone>(Other)) 
	{
		RegisterDrone(Drone);
	}
}

/// <summary>
/// When drone leaves trigger, unregister it from this command center
/// </summary>
/// <param name="Other"></param>
void ACommandCenter::NotifyActorEndOverlap(AActor* Other)
{
	if (ADrone* Drone = Cast<ADrone>(Other))
	{
		UnregisterDrone(Drone);
	}
}

/// <summary>
/// Registers given drone to this command center. 
/// </summary>
/// <param name="Drone">Drone to register</param>
void ACommandCenter::RegisterDrone(ADrone* Drone)
{
	//Don't register drone we already have registered
	if (DronesInControl.Find(Drone) != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already have registered drone"));
		return;
	}		
	
	Drone->UnregisterFromCurrentCommandCenter();
	DronesInControl.Add(Drone);
	Drone->SetCommandCenter(this);
}

/// <summary>
/// Unregisters given drone from this command center.
/// </summary>
/// <param name="Drone">Drone to unregister</param>
void ACommandCenter::UnregisterDrone(ADrone* Drone)
{
	int32 DroneIndex = DronesInControl.Find(Drone);

	//we don't have this drone registered
	if (DroneIndex == INDEX_NONE)
		return;

	Drone->SetCommandCenter(nullptr);
	DronesInControl.RemoveAtSwap(DroneIndex);
}

/// <summary>
/// Spawns drones in the postion of the command center and generates a random order for it.
/// </summary>
void ACommandCenter::SpawnDrones()
{
	for (int i = 0; i < NumberOfSpawnedDrones; i++)
	{
		//spawn drone
		FActorSpawnParameters SpawnParams;
		ADrone* NewDrone = GetWorld()->SpawnActor<ADrone>(DroneBlueprint, GetTransform(), SpawnParams);
		
		//set it's position
		FVector Position = NewDrone->GetTransform().GetLocation();
		Position.Z = 215;
		NewDrone->SetActorLocation(Position);

		//generate random order and give it to a drone
		FDroneOrder NewOrder = FOrderGeneration::GenerateOrder(MinDistance, MaxDistance);
		NewDrone->Init(DroneTypeToSpawn, NewOrder);
		RegisterDrone(NewDrone);
	}
}

/// <summary>
/// Generates random order for each drone in range
/// </summary>
void ACommandCenter::GiveOrders()
{
	for (int i = 0; i < DronesInControl.Num(); i++)
	{
		ADrone* Drone = DronesInControl[i];
		FDroneOrder NewOrder = FOrderGeneration::GenerateOrder(MinDistance, MaxDistance);
		Drone->SetNewOrder(NewOrder);
	}
}

#if WITH_EDITOR
/// <summary>
/// Updates trigger range when the ControlRange parameter is updated in Details of the Command Center in editor
/// </summary>
/// <param name="e"></param>
void ACommandCenter::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACommandCenter, ControlRange)) {
		TriggerComponent = FindComponentByClass<USphereComponent>();
		TriggerComponent->SetSphereRadius(ControlRange);
	}
}
#endif


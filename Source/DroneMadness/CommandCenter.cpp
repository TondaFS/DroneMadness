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

	SpawnDrones(DroneTypeToSpawn, NumberOfSpawnedDrones);

	TriggerComponent = FindComponentByClass<USphereComponent>();
	TriggerComponent->SetSphereRadius(ControlRange);
}

void ACommandCenter::NotifyActorBeginOverlap(AActor* Other)
{
	if (ADrone* Drone = Cast<ADrone>(Other)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Drone entered trigger"));
		RegisterDrone(Drone);
	}
}

void ACommandCenter::NotifyActorEndOverlap(AActor* Other)
{
	if (ADrone* Drone = Cast<ADrone>(Other))
	{
		UE_LOG(LogTemp, Warning, TEXT("Drone exited trigger"));
		UnregisterDrone(Drone);
	}
}

// Called every frame
void ACommandCenter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
	UE_LOG(LogTemp, Warning, TEXT("Drone registered"));
}

void ACommandCenter::UnregisterDrone(ADrone* Drone)
{
	int32 DroneIndex = DronesInControl.Find(Drone);

	//we don't have this drone registered
	if (DroneIndex == INDEX_NONE)
		return;

	Drone->SetCommandCenter(nullptr);
	DronesInControl.RemoveAtSwap(DroneIndex);
	UE_LOG(LogTemp, Warning, TEXT("Drone un-registered"));
}

void ACommandCenter::SpawnDrones(EDroneType Type, int32 Count)
{
	for (int i = 0; i < Count; i++)
	{
		FActorSpawnParameters SpawnParams;
		ADrone* NewDrone = GetWorld()->SpawnActor<ADrone>(DroneBlueprint, GetTransform(), SpawnParams);
		NewDrone->DroneType = Type;
		
		FDroneOrder NewOrder = FOrderGeneration::GenerateOrder(MinDistance, MaxDistance);
		NewDrone->Init(DroneTypeToSpawn, NewOrder);
		RegisterDrone(NewDrone);
	}
}

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


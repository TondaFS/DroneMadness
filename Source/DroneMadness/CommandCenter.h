// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneType.h"
#include "Drone.h"
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include "CommandCenter.generated.h"


UCLASS()
/// <summary>
/// Class representing CommandCenter on map.
/// Command center spawns new drones with given type, registers and unregisters drones and gives orders to
/// drones in range.
/// </summary>
class DRONEMADNESS_API ACommandCenter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommandCenter();
		
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Command Center")
	float ControlRange;

	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Orders")
	/// <summary>
	/// How often the center gives orders.
	/// </summary>
	float BroadcastRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
	/// <summary>
	/// Min distance for order generation
	/// </summary>
	float MinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
	/// <summary>
	/// Max distance for order genration
	/// </summary>
	float MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TEnumAsByte<EDroneType> DroneTypeToSpawn;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Spawn")
	int32 NumberOfSpawnedDrones;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Spawn")
	float SpawnRate;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Spawn")
	/// <summary>
	/// Reference for DroneBlueprint to spawn
	/// </summary>
	TSubclassOf<ADrone> DroneBlueprint;
		
private:
	UPROPERTY(VisibleAnywhere, Category = "Drones")
	TArray<ADrone*> DronesInControl;
	USphereComponent* TriggerComponent;

	//handles for timers to spawning and giving orders
	FTimerHandle SpawnHandle;
	FTimerHandle OrdersHandle;

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
#endif

public:	
	void RegisterDrone(ADrone* Drone);
	void UnregisterDrone(ADrone* Drone);

private:
	void SpawnDrones();
	void GiveOrders();

};

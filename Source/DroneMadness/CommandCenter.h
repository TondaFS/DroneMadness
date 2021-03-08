// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneType.h"
#include "Drone.h"
#include "CommandCenter.generated.h"

UCLASS()
class DRONEMADNESS_API ACommandCenter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommandCenter();
		
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Command Center")
		float ControlRange;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Orders")
		float BroadcastRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
		float MinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
		float MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TEnumAsByte<EDroneType> DroneTypeToSpawn;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Spawn")
		int32 NumberOfSpawnedDrones;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Spawn")
		float SpawnRate;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Spawn")
	TSubclassOf<ADrone> DroneBlueprint;
		
private:
	UPROPERTY(VisibleAnywhere, Category = "Drones")
	TArray<ADrone*> DronesInControl;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RegisterDrone(ADrone* Drone);
	void UnregisterDrone(ADrone* Drone);

private:
	void SpawnDrones(EDroneType Type, int32 Count);
	void GiveOrders();

};

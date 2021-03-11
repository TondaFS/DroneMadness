// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneType.h"
#include "DroneOrder.h"
#include "Drone.generated.h"

//forward declaration
class ACommandCenter;

UCLASS()
/// <summary>
/// Class representing Drone actor on the map.
/// Drone has a type and speed and moves to the destination of its current order.
/// </summary>
class DRONEMADNESS_API ADrone : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADrone();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
		TEnumAsByte<EDroneType> DroneType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	FDroneOrder CurrentOrder;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Generation")
	/// <summary>
	/// Min distance to go for randomly generated order
	/// </summary>
	float MinDistance;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Generation")
	/// <summary>
	/// Max distance to go for randomly generated order
	/// </summary>
	float MaxDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	ACommandCenter* CurrentCommandCenter;

	/// <summary>
	/// Delegate for event when drone registers to a new command center.
	/// </summary>
	/// <param name="">Event definition</param>
	/// <param name="">bool determining if the commandCenter is null (false) or not (true)</param>
	/// <param name=""></param>
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewCommandCenterRegistered, bool, CommandCenter);
	/// <summary>
	/// Event called when drone register to a new command center.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Drone Events")
		FNewCommandCenterRegistered OnNewCommandCenterRegistered;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDroneInitiated);
	/// <summary>
	/// Event called when drone initiates itself after spawning
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Drone Events")
		FDroneInitiated OnDroneInitiated;

private:
	bool IsHit;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector MovementDestination;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementCloseCheckDistance;
			
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetNewOrder(FDroneOrder NewOrder);
	void OnHit();
	void Init(EDroneType Type, FDroneOrder Order);
	void UnregisterFromCurrentCommandCenter();
	void SetCommandCenter(ACommandCenter* CommandCenter);

private:
	void GenerateNewOrder();
	void MoveDrone(float DeltaTime);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneType.h"
#include "DroneOrder.h"
#include "Drone.generated.h"

//forward declaration
class ACommandCenter;

UCLASS()
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
		float MinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Generation")
		float MaxDistance;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
		ACommandCenter* CurrentCommandCenter;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewCommandCenterRegistered, bool, CommandCenter);
	UPROPERTY(BlueprintAssignable, Category = "Drone Events")
		FNewCommandCenterRegistered OnNewCommandCenterRegistered;
	//FNewCommandCenterRegistered& OnNewCommandCenterRegistered() { return NewCommandCenterRegistered; }

private:
	bool IsHit;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector MovementDestination;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementCloseCheckDistance;
		
	//FNewCommandCenterRegistered NewCommandCenterRegistered;
	

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
};

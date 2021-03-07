// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroneType.h"
#include "DroneOrder.h"
#include "Drone.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	FDroneOrder CurrentOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Generation")
		float MinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Generation")
		float MaxDistance;

private:
	bool IsHit;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector MovementDestination;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementCloseCheckDistance;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetNewOrder(FDroneOrder NewOrder);
	void OnHit();

private:
	void GenerateNewOrder();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SailComp.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API USailComp : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	USailComp();
	UPROPERTY(EditAnywhere)
	float SailAngle;
	UPROPERTY(EditAnywhere)
	float MaxSailStrength = 3.0f;
	UPROPERTY(EditAnywhere)
	float MinSailStrength = 1.0f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class ARaftGameState* RaftGameState;
	FVector3d WindDirection;
	float WindStrength;

	FVector MyDirection;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	void ChangeStrength(float myStrength);
	float CompareDirection(FVector3d myDir, FVector3d windDir);
};

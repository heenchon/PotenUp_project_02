// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishingFloat.generated.h"

UCLASS()
class PROJECT_02_API AFishingFloat : public AActor
{
	GENERATED_BODY()

public:
	AFishingFloat();

	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere)
	float ThrowDuration = 1.0f;
	UPROPERTY(EditAnywhere)
	float MaxThrowHeight = 50.0f;
	bool bIsThrowing;
	FVector StartLocation;
	FVector FishingLocation;
	
	float CurTime;

	bool bIsFishing;
	float FishingTime;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Throwing(float deltaTime);
	void StartFishing();
	void Fishing(float deltaTime);
	void GetFish();
	void StartThrow(const FVector& startLoc, const FVector& endLoc);
};

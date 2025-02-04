// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FishingFloat.generated.h"

class UWidgetComponent;
class AFishingRod;

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
	class UBoxComponent* Collision;

	UPROPERTY()
	class APawn* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFishingRod* FishingRod;

	UPROPERTY(EditAnywhere, Category = "Curve Animation")
	UCurveFloat* ZCurve;
	UPROPERTY()
	FTimeline ZTimeline;
	
	bool bIsThrowing;
	FVector StartLocation;
	float Gravity = -980.0f;
	FVector Velocity;

	bool bIsWaiting = false;
	float WaitTime;
	bool bIsFish = false;
	UPROPERTY(EditAnywhere)
	float FishingTime = 3.0f;

	float CurTime;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult &SweepResult
	);

	void StartThrow(const FVector& startLoc, float& power);
	void Throwing(float deltaTime);
	void StartWaiting();
	void Waiting(float deltaTime);
	void Fishing(float deltaTime);
	void SetFishingRod(AFishingRod* fishingRod);
	void FishBate(float value);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> FishWidget;
};


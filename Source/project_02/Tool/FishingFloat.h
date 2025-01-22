// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishingFloat.generated.h"


class ABasePlayerState;

UCLASS()
class PROJECT_02_API AFishingFloat : public AActor
{
	GENERATED_BODY()

public:
	AFishingFloat();
	ABasePlayerState* PS;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APawn* Player;
	
	bool bIsThrowing;
	FVector StartLocation;
	FVector FishingLocation;
	UPROPERTY(EditAnywhere)
	float ThrowDuration = 0.5f;
	UPROPERTY(EditAnywhere)
	float MaxThrowHeight = 50.0f;
	
	bool bIsFishing;
	float FishingTime;

	float CurTime;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Throwing(float deltaTime);
	void StartFishing();
	void Fishing(float deltaTime);
	void GetFish();
	void StartThrow(const FVector& startLoc, float& power);
};

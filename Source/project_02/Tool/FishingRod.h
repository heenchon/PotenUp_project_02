// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "FishingRod.generated.h"

class AFishingFloat;

UCLASS()
class PROJECT_02_API AFishingRod : public AInteractiveItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishingRod();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* RodMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RodPoint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AFishingFloat> Float;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFishingFloat> FloatClass;

	bool bIsFish;
	bool bIsWaiting;
	bool bIsCharging;
	float Power;
	float MaxPower = 700.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void StartInteractive() override;
	virtual void OnInteractiveHold(float DeltaTime) override;
	virtual bool EndInteractive() override;
	void ChargeStart();
	void Charging(float deltaTime);
	void ChargeEnd();
	void StartFishing();
	void GetFish();
	void FishingFail();
};



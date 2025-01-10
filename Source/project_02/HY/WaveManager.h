// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

UCLASS()
class PROJECT_02_API AWaveManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaveManager();
	UPROPERTY(EditAnywhere)
	class AWaterBodyOcean* BodyOcean;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGerstnerWaterWaveGeneratorBase> GWaterWaves;

	UPROPERTY(EditAnywhere, Category = "wave")
	float WaveLength;
	UPROPERTY(EditAnywhere, Category = "wave")
	float Amplitude;
	UPROPERTY(EditAnywhere, Category = "wave")
	float Steepness;
	UPROPERTY(EditAnywhere, Category = "wave")
	float WaveSpeed;
	UPROPERTY(EditAnywhere, Category = "wave")
	FVector Direction;
	UPROPERTY(EditAnywhere, Category = "wave")
	FVector2D WaveVector;

	void ChangeWaves();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void GenerateGerstnerWaves(TArray<FGerstnerWave>&OutWaves) const;
};

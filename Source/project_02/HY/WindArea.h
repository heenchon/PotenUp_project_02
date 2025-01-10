// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindArea.generated.h"

UCLASS()
class PROJECT_02_API AWindArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWindArea();

	UPROPERTY(EditAnywhere, Category = "Wind")
	FVector WindDirection = {1,0,0};
	
	UPROPERTY(EditAnywhere, Category = "Wind")
	float WindStrength = 150.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

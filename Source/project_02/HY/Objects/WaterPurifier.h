// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceObjects.h"
#include "WaterPurifier.generated.h"

UCLASS()
class PROJECT_02_API AWaterPurifier : public APlaceObjects
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterPurifier();

	bool bIsPurified;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AUsable_Item* input) override;
	virtual void ProcessComplete() override;
};



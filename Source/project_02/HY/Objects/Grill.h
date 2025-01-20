// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceObjects.h"
#include "Grill.generated.h"

UCLASS()
class PROJECT_02_API AGrill : public APlaceObjects
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrill();

	bool bIsFood;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact(AUsable_Item* input) override;
	virtual void ProcesComplete() override;
};

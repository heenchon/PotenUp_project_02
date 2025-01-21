// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usable_Item.h"
#include "FishRaw.generated.h"

class USurvivalComponent;

UCLASS()
class PROJECT_02_API AFishRaw : public AUsable_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishRaw();
	
	bool bIsCooked;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void PutOnGrill();
};

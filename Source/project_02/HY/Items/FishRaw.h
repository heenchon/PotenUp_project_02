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
	AFishRaw();

protected:

public:
	void PutOnGrill();
};

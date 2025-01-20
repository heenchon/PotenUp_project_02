// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usable_Item.h"
#include "FishCooked.generated.h"

class USurvivalComponent;

UCLASS()
class PROJECT_02_API AFishCooked : public AUsable_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishCooked();

	UPROPERTY(EditAnywhere)
	USurvivalComponent* SurvivalComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Use() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usable_Item.h"
#include "Cup.generated.h"

class USurvivalComponent;

UCLASS()
class PROJECT_02_API ACup : public AUsable_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACup();
	
	USurvivalComponent* SurvivalComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
};

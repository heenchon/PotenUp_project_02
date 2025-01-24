// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceObjects.h"
#include "Radio.generated.h"

UCLASS()
class PROJECT_02_API ARadio : public APlaceObjects
{
	GENERATED_BODY()

public:
	ARadio();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> EndingWidgetClass;
	UPROPERTY()
	UUserWidget* EndingUI;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
};

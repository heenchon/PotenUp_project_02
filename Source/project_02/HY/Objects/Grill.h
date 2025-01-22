// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceObjects.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "Grill.generated.h"


UCLASS()
class PROJECT_02_API AGrill : public APlaceObjects
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrill();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* FoodPoint;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* RawFoodMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AUsable_Item> FishCookedTemp;
	
	bool bIsCooking;
	bool bIsCooked = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact(AUsable_Item* input, int curItemIndex) override;
	virtual void ProcessComplete() override;
};

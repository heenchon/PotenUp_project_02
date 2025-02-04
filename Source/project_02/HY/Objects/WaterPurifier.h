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
		
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WaterMesh;
	
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Ocean;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Fresh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundWave> WaterBoilingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BoilingSoundComponent;
	
	bool bIsPurified;
	
	virtual FString GetDisplayText() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AUsable_Item* input, int curItemIndex) override;
	virtual void ProcessComplete() override;
};



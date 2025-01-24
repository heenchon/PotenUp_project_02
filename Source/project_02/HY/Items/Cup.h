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

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WaterMesh;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Ocean;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Fresh;
	
	UPROPERTY()
	class APawn* Player;
	UPROPERTY()
	class APlayerCameraManager* CamManager;
	UPROPERTY()
	USurvivalComponent* SurvivalComponent;
	
	static bool bIsFresh;
	static bool bIsSea;
	static bool bIsEmpty;
	static bool bLookingSea;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	void FillSeaWater();
	void FillFreshWater();
	void EmptyCup();
	bool IsLookingSea();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chaos/PerCellBuoyancy.h"
#include "GameFramework/Actor.h"
#include "BuoyancyComponent.h"
#include "Raft.generated.h"

UCLASS()
class PROJECT_02_API ARaft : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARaft();
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY()
	UBuoyancyComponent* Buoyancy;

	UPROPERTY(EditAnywhere, Category = "Move")
	FVector3d WindDirection = {1,0,0};
	
	UPROPERTY(EditAnywhere, Category = "Move")
	float WindStrength = 300.0f;
	UPROPERTY(visibleAnywhere)
	float SailStrength = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

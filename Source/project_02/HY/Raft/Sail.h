// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Raft.h"
#include "Sail.generated.h"

UCLASS()
class PROJECT_02_API ASail : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASail();
	UPROPERTY(EditAnywhere)
	float SailAngle;
	UPROPERTY(EditAnywhere)
	float MaxSailStrength = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ARaft* Raft;
	FVector3d WindDirection;
	float WindStrength;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeStrength(float myStrength);
	float CompareDirection(FVector3d myDir, FVector3d windDir);
};

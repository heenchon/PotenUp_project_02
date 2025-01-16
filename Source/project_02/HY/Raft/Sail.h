// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sail.generated.h"

UCLASS()
class PROJECT_02_API ASail : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASail();
	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* SailMesh;
	//TODO: 에셋 찾으면 제거
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* Arrow;

	UPROPERTY(EditAnywhere)
	float MaxSailStrength = 6.0f;
	UPROPERTY(EditAnywhere)
	double RotationMultiplier = 2.0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ARaftGameState* RaftGameState;
	FVector3d WindDirection;
	float WindStrength;
	
	UPROPERTY(EditAnywhere)
	class ARaft* Raft;
	UPROPERTY(EditAnywhere)
	class AController* PlayerController;
	float PlayerYawOrigin;
	float SailYawOrigin;
	
	float MinSailStrength;
	FVector MyDirection;
	bool bSailOn = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ChangeStrength(float myStrength);
	float CompareDirection(FVector3d myDir, FVector3d windDir);
	void SailToggle();
	void RotateSail();
	void RotateInit(float yawValue);
	void SetRaft(ARaft* raft);
};

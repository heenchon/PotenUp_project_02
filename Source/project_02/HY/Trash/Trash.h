// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trash.generated.h"

UCLASS()
class PROJECT_02_API ATrash : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrash();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	class UBuoyancyComponent* Buoyancy;

	//TODO: 윈드 매니저 추가 후 cpp 수정
	UPROPERTY(EditAnywhere)
	class ARaft* Raft;
	FVector WindDirection;
	float WindStrength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

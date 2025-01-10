// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ESharkState.h"
#include "SharkAI.generated.h"

UCLASS()
class PROJECT_02_API ASharkAI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASharkAI();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	AActor* Player; AActor* Raft;
	
	//타겟 상태
	UPROPERTY(EditAnywhere)
	FVector TargetLocation;
	UPROPERTY(EditAnywhere)
	bool PlayerIsWater;
	
	//상어 상태
	UPROPERTY()
	ESharkState CurrentState;

	//상어 속성
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkBasicSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//상태 전환
	void ChangeState(ESharkState newState);
	//상태별 함수
	void Idle(float DeltaTime);
	void MoveToTarget(float DeltaTime, FVector targetLoc);
	void AttackPlayer(float DeltaTime);
	void AttackRaft(float DeltaTime);
	void Runaway(float DeltaTime);
};





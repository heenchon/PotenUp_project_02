// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SharkStateMachine.h"
#include "SharkAI.generated.h"


UCLASS()
class PROJECT_02_API ASharkAI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASharkAI();

	AActor* Player;
	AActor* Raft;
	
	UPROPERTY()
	USharkStateMachine* StateMachine;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	//상어 속성
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkBasicSpeed;
	float SharkAttackSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
// #include "../SharkState.h"
#include "SharkStateMachine.generated.h"

enum class ESharkState : uint8;
class USharkIdle;

class ASharkAI;

UCLASS()
class PROJECT_02_API USharkStateMachine : public UObject
{
	GENERATED_BODY()
public:
	USharkStateMachine();
	// ~SharkStateMachine();
	
	UPROPERTY()
	UObject* CurState;
	UPROPERTY()
	USharkIdle* Idle;
	// UPROPERTY()
	// USharkMovingToRaft* MoveToRaft;
	// UPROPERTY()
	// USharkAttackingRaft* AttackRaft;

	UPROPERTY()
	float SharkBasicSpeed;
	float SharkAttackSpeed;

	// void Initialize();
	void ChangeState(UObject* newState, ASharkAI* Shark);
	// UObject GetStateObject(ESharkState State);
	void Update(ASharkAI* Shark,float DeltaTime);
};

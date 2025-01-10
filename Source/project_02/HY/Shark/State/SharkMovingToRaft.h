// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../SharkState.h"
#include "SharkMovingToRaft.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_02_API USharkMovingToRaft : public UObject, public ISharkState
{
	GENERATED_BODY()
	
public:
	FVector TargetLocation;
	
	virtual void Enter(ASharkAI* Shark, USharkStateMachine* FSM) override;
	virtual void Update(ASharkAI* Shark, float DeltaTime) override;
	virtual void Exit(ASharkAI* Shark) override;
	// virtual void TestFSM(USharkStateMachine* StateMachine)  override;

};

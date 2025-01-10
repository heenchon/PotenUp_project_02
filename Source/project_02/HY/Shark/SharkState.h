// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SharkState.generated.h"

class USharkStateMachine;
class ASharkAI;

UINTERFACE(Blueprintable) 
class PROJECT_02_API USharkState : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_02_API ISharkState
{
	GENERATED_BODY()

public:
	virtual void Enter(ASharkAI* Shark, USharkStateMachine* FSM) = 0;
	virtual void Update(ASharkAI* Shark, float DeltaTime) = 0;
	virtual void Exit(ASharkAI* Shark) = 0;
	// virtual ESharkState GetState();
};

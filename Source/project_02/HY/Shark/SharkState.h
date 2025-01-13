// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SharkState.generated.h"

// 전방 선언 (Forward Declaration)
class ASharkAI;

UENUM(BlueprintType)
enum class ESharkState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	MoveToRaft UMETA(DisplayName = "MoveToRaft"),
	AttackRaft UMETA(DisplayName = "AttackRaft"),
	RunAway UMETA(DisplayName = "RunAway"),
	FindPlayer UMETA(DisplayName = "FindPlayer"),
	MoveToPlayer UMETA(DisplayName = "MoveToPlayer"),
	AttackPlayer UMETA(DisplayName =  "AttackPlayer")
};


UINTERFACE(Blueprintable) 
class PROJECT_02_API USharkState : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_02_API ISharkState
{
	GENERATED_BODY()

public:
	virtual void Enter(ASharkAI* Shark) = 0;
	virtual void Update(ASharkAI* Shark) = 0;
	virtual void Exit(ASharkAI* Shark) = 0;
};

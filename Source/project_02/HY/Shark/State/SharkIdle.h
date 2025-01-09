// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../SharkState.h"
#include "SharkIdle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_02_API USharkIdle : public UObject, public ISharkState
{
	GENERATED_BODY()

public:
	float CurTime = 0.0f;
	float AttackDuration = 5.0f;
	
	virtual void Enter(ASharkAI* Shark) override;
	virtual void Update(ASharkAI* Shark, float DeltaTime) override;
	virtual void Exit(ASharkAI* Shark) override;
};

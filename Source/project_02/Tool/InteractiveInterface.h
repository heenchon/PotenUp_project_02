// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

UINTERFACE()
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_02_API IInteractiveInterface
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual FString GetDisplayText() const { return DisplayText; }
	
protected:
	FString DisplayText;
};

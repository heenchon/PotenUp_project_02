// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WindInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWindInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_02_API IWindInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetWindDirection(FVector& NewDir) = 0;
	virtual void SetWindStrength(float& NewStr) = 0;

	virtual FVector GetWindDirection() const = 0;
	virtual  FVector GetWindStrength() const = 0;
};

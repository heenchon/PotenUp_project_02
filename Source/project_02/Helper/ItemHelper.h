// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemHelper.generated.h"

struct FItemInfoData;
/**
 * 
 */
UCLASS()
class PROJECT_02_API UItemHelper : public UObject
{
	GENERATED_BODY()

public:
	static FItemInfoData GetItemInfoById(const UWorld* World, const uint16 Id);
};

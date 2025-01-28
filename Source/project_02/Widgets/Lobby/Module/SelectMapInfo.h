// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapInfo.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_02_API USelectMapInfo : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMapName(const FString& NewMapName) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MapName;
};

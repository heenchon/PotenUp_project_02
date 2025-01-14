// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "BaseDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_02_API UBaseDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	FORCEINLINE TObjectPtr<UUserWidget> GetOriginWidget() const
	{
		return OriginWidget;
	}
	FORCEINLINE void SetOriginWidget(UUserWidget* NewOriginWidget)
	{
		OriginWidget = NewOriginWidget;
	}
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> OriginWidget;
};

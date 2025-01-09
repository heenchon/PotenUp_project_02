// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryList.generated.h"

class UInventorySlot;
/**
 * 
 */
UCLASS()
class PROJECT_02_API UInventoryList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 MaxInventoryCount = 10;
	// 인벤토리 정보 자체는 외부 PC or PS에서 가져올 것

	UPROPERTY(EditDefaultsOnly, Cateogory = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventorySlot> ItemSlot;
};

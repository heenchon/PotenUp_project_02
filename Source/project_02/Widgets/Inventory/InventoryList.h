// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryList.generated.h"

class UGridPanel;
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
	uint8 ItemListColumn;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 MaxItemCount;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventorySlot> ItemSlotClass;

	UPROPERTY()
	TArray<TObjectPtr<UInventorySlot>> ItemSlotList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> ItemGridList;
};

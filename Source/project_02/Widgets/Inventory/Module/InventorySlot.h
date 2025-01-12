// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UBorder;
struct FItemMetaInfo;
class UTextBlock;
class UImage;


UCLASS()
class PROJECT_02_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSlotInfo(const FItemMetaInfo& ItemMetaInfo) const;

	void SetSelected(const bool bIsSelected) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SelectedBorder;
};
 
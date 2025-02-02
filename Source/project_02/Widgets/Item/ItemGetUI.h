// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "project_02/DataTable/ItemInfoData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemGetUI.generated.h"

class UItemUI;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_02_API UItemGetUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;

private:
	UItemUI* MainItemUI;
	FTimerHandle Timer;

public:
	void DestroyItemGetUI();

	void InitItemUI(UItemUI* mainUI,uint8 count,FString displayName,TObjectPtr<UTexture2D> texture);
	
	void SetItemCount(uint8 count);
	void SetItemName(FString displayName);
	void SetItemThumbnail(TSoftObjectPtr<UTexture2D> texture);
};

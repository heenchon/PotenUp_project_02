// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

class UVerticalBox;
class USizeBox;
class UItemGetUI;
/**
 * 
 */
UCLASS()
class PROJECT_02_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemGetUI> ItemGetUIClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> ItemListWrapper;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ItemListBox;
	
	TArray<UItemGetUI*> ItemGetUIList;
	
public:
	void AddItemGetUI(uint8 count,FString displayName,TSoftObjectPtr<UTexture2D> texture);
	void RemoveItemGetUI(UItemGetUI* itemGetUI);
	void UpdateItemListPosition();
};



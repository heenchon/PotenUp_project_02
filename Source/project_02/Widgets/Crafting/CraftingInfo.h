// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CraftingInfo.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECT_02_API UCraftingInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemName(const FText& NewItemName) const { ItemName->SetText(NewItemName); }
	void SetItemThumbnail(const TSoftObjectPtr<UTexture2D>& Thumbnail);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
};

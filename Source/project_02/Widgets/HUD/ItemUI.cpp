// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUI.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "project_02/Widgets/Item/ItemGetUI.h"

void UItemUI::AddItemGetUI(uint8 count, FString displayName,TSoftObjectPtr<UTexture2D> texture)
{
	UItemGetUI* itemGetUI = CreateWidget<UItemGetUI>(GetWorld(), ItemGetUIClass);
	
	if (itemGetUI)
	{
		//SoftPtr을 일반 Ptr로. 동기 방식
		TSoftObjectPtr<UTexture2D> SoftTexture = texture; // SoftObjectPtr에 할당된 값
		UTexture2D* LoadedTexture = SoftTexture.LoadSynchronous();

		TObjectPtr<UTexture2D> ObjectTexture = LoadedTexture;
		
		itemGetUI->InitItemUI(this,count,displayName,ObjectTexture);
		ItemGetUIList.Add(itemGetUI);
		
		ItemListBox->InsertChildAt(0, itemGetUI);

		UVerticalBoxSlot* newSlot = Cast<UVerticalBoxSlot>(itemGetUI->Slot);
		if (newSlot)
		{
			newSlot->SetHorizontalAlignment(HAlign_Center);
		}
		
		itemGetUI->AddToViewport();
	}
	
	UpdateItemListPosition();
}

void UItemUI::RemoveItemGetUI(UItemGetUI* itemGetUI)
{
	ItemGetUIList.Remove(itemGetUI);
	UpdateItemListPosition();
}

void UItemUI::UpdateItemListPosition()
{
	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(ItemListWrapper->Slot);
	if (canvasSlot)
	{
		FMargin newOffsets = canvasSlot->GetOffsets();
		newOffsets.Top = -ItemGetUIList.Num() * 108.0f; // ItemGetUI Y 사이즈
		canvasSlot->SetOffsets(newOffsets);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryList.h"

#include "Components/GridPanel.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"

void UInventoryList::NativePreConstruct()
{
	if (ItemSlotClass)
	{
		for (int i = 0; i < MaxItemCount; i++)
		{
			if (UInventorySlot* NewInventorySlot = CreateWidget<UInventorySlot>(this, ItemSlotClass))
			{
				ItemSlotList.Add(NewInventorySlot);
				ItemGridList->AddChildToGrid(NewInventorySlot, i / ItemListColumn, i % ItemListColumn);
			}
		}
	}
}

#include "InventoryHotSlot.h"

#include "Components/GridPanel.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"

void UInventoryHotSlot::SetInventoryArray(const uint8 NewCount)
{
	if (ItemSlotClass)
	{
		for (int i = 0; i < NewCount; i++)
		{
			if (UInventorySlot* NewInventorySlot = CreateWidget<UInventorySlot>(this, ItemSlotClass))
			{
				ItemSlotList.Add(NewInventorySlot);
				ItemGridList->AddChildToGrid(NewInventorySlot, 0, i);
			}
		}
	}
}


void UInventoryHotSlot::NativePreConstruct()
{
	SetInventoryArray(SlotCount);
}

void UInventoryHotSlot::NativeConstruct()
{
	// PreConstruct에서 진행하는 것은 개발용 툴이기에 의미 있지는 않다.
	// 즉 SlotCount는 개발용 변수 값
	ItemGridList->ClearChildren();
	
	if (const ABasePlayerState* PS = Cast<ABasePlayerState>(GetOwningPlayerState()))
	{
		SetInventoryArray(PS->GetHotSlotCount());
		
		for (int i = 0; i < PS->GetHotSlotCount(); i++)
		{
			if (PS->GetPlayerHotSlotList().IsValidIndex(i))
			{
				const UInventorySlot* CurrentItemSlot = Cast<UInventorySlot>(ItemGridList->GetChildAt(i));
				CurrentItemSlot->SetSlotInfo(PS->GetPlayerHotSlotList()[i]);
			}
		}
	}
}
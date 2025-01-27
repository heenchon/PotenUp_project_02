#include "InventoryList.h"

#include "Components/GridPanel.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"

void UInventoryList::SetInventoryArray(const uint8 NewCount)
{
	if (ItemSlotClass)
	{
		for (int i = 0; i < NewCount; i++)
		{
			if (UInventorySlot* NewInventorySlot = CreateWidget<UInventorySlot>(this, ItemSlotClass))
			{
				ItemSlotList.Add(NewInventorySlot);
				ItemGridList->AddChildToGrid(NewInventorySlot, i / ItemListColumn, i % ItemListColumn);
			}
		}
	}
}

void UInventoryList::UpdateInventoryArray()
{
	if (const ABasePlayerState* PS = Cast<ABasePlayerState>(GetOwningPlayerState()))
	{
		
		SetInventoryArray(PS->GetInventorySlotCount());
		
		for (int i = PS->GetHotSlotCount(); i < PS->GetTotalSlotCount(); i++)
		{
			// 실제 인벤토리와 다르게 인벤토리 UI는 핫슬롯을 제외한 만큼의 인벤토리다.
			UInventorySlot* CurrentItemSlot = Cast<UInventorySlot>(ItemGridList->GetChildAt(i - PS->GetHotSlotCount()));
			CurrentItemSlot->SetSlotInfo(PS->GetPlayerInventoryList()[i]);
			CurrentItemSlot->SetIndex(i);
		}
	}
}

void UInventoryList::NativePreConstruct()
{
	SetInventoryArray(SlotCount);
}

void UInventoryList::NativeConstruct()
{
	// PreConstruct에서 진행하는 것은 개발용 툴이기에 의미 있지는 않다.
	// 즉 SlotCount는 개발용 변수 값
	ItemGridList->ClearChildren();
	UpdateInventoryArray();
}

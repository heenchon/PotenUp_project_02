#include "InventoryHotSlot.h"

#include "Components/GridPanel.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"


void UInventoryHotSlot::ChangeSelectedHotSlot(const uint8 PrevSlot, const uint8 NextSlot)
{
	// TODO: Cast를 자체적으로 만들기 위해 IsA와 static_cast를 섞어서 사용해보기
	if (ItemGridList->GetChildAt(PrevSlot)->IsA(UInventorySlot::StaticClass()))
	{
		const UInventorySlot* PrevItemSlot = static_cast<UInventorySlot*>(ItemGridList->GetChildAt(PrevSlot));
		PrevItemSlot->SetSelected(false);
	}
	
	if (ItemGridList->GetChildAt(NextSlot)->IsA(UInventorySlot::StaticClass()))
	{
		const UInventorySlot* NextItemSlot = static_cast<UInventorySlot*>(ItemGridList->GetChildAt(NextSlot));
		NextItemSlot->SetSelected(true);
	}
}

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
			// TODO: 게임 시작 시 처음에는 0으로 시작한다는 하드코딩
			// 추후 코드 이전할 필요가 있음.
			if (i == 0)
			{
				const UInventorySlot* CurrentItemSlot = Cast<UInventorySlot>(ItemGridList->GetChildAt(i));
				CurrentItemSlot->SetSelected(true);
			}
			
			if (PS->GetPlayerHotSlotList().IsValidIndex(i))
			{
				const UInventorySlot* CurrentItemSlot = Cast<UInventorySlot>(ItemGridList->GetChildAt(i));
				CurrentItemSlot->SetSlotInfo(PS->GetPlayerHotSlotList()[i]);
			}
		}
	}
}

void UInventoryHotSlot::UpdateInventoryArray()
{
	const ABasePlayerState* PS = Cast<ABasePlayerState>(GetOwningPlayerState());
	
	for (int i = 0; i < PS->GetHotSlotCount(); i++)
	{
		if (PS->GetPlayerHotSlotList().IsValidIndex(i))
		{
			const UInventorySlot* CurrentItemSlot = Cast<UInventorySlot>(ItemGridList->GetChildAt(i));
			CurrentItemSlot->SetSlotInfo(PS->GetPlayerHotSlotList()[i]);
		}
	}
}

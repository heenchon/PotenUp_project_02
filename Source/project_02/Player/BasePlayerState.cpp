#include "BasePlayerState.h"

#include "BasePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Helper/ItemHelper.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"
#include "project_02/Widgets/Inventory/InventoryHotSlot.h"

ABasePlayerState::ABasePlayerState()
{
	
}

void ABasePlayerState::InitializeData()
{
	const FItemMetaInfo EmptyItem;
	PlayerInventoryList.Init(EmptyItem, GetTotalSlotCount());
	
	// TODO: 제거해야 할 테스트 코드
	FItemMetaInfo NewItem;
	NewItem.SetId(1);
	NewItem.SetCurrentCount(1);

	PlayerInventoryList[0] = NewItem;
}


void ABasePlayerState::BeginPlay()
{
}

uint32 ABasePlayerState::AddItemToInventory(const uint16 Index, const FItemMetaInfo& ItemInfo)
{
	const FItemInfoData& ItemInfoById = UItemHelper::GetItemInfoById(GetWorld(), ItemInfo.GetId());

	// CurrentItemCount는 우선은 총 합으로 가지고 있는 아이템 수를 의미한다.
	const uint16 CurrentItemCount = PlayerInventoryList[Index].GetCurrentCount() +
		ItemInfo.GetCurrentCount();
		
	const uint16 NextSetMainItemCount = UKismetMathLibrary::Min(
	CurrentItemCount
	, ItemInfoById.GetMaxItemCount());
		
	// 우선 최대 갯수를 차지하면 최대 갯수를 넣고, 그게 아니면 그냥 더한 갯수를 넣어줌
	if (PlayerInventoryList[Index].GetId() == 0)
	{
		PlayerInventoryList[Index].SetId(ItemInfo.GetId());
		PlayerInventoryList[Index].SetMetaData(ItemInfo.GetMetaData());
	}
	PlayerInventoryList[Index].SetCurrentCount(NextSetMainItemCount);

	// 최대 값 만큼 넣어도 남는 경우가 존재한다.
	int32 RemainCount = CurrentItemCount - ItemInfoById.GetMaxItemCount();
	
	// 이 경우는 아이템에 여분이 있어 더 추가해야 하는 경우를 의미한다.
	if (RemainCount > 0)
	{
		// 이미 꽉 찬 경우여도 여분의 칸 검색을 위해
		// 처음부터 다시 탐색해서 남은 값 들을 순차적으로 넣어준다.
		for (int i = 0; i < GetTotalSlotCount(); i++)
		{
			if (RemainCount == 0)
			{
				break;
			}
			// 단순히 i번째에 값이 없는 경우에 대해서는 값을 넣어준다.
			if (PlayerInventoryList[i].GetId() == 0)
			{
				FItemMetaInfo NewItemInfo;
				NewItemInfo.SetId(ItemInfo.GetId());
				NewItemInfo.SetMetaData(ItemInfo.GetMetaData());
				NewItemInfo.SetCurrentCount(UKismetMathLibrary::Min(
					RemainCount, ItemInfoById.GetMaxItemCount()));
					
				PlayerInventoryList[i] = NewItemInfo;
				RemainCount -= UKismetMathLibrary::Min(RemainCount, ItemInfoById.GetMaxItemCount());
			}
		}
	}

	return RemainCount > 0 ? RemainCount : 0;
}


void ABasePlayerState::AddItem(const FItemMetaInfo& ItemInfo)
{
	const FItemInfoData& ItemInfoById = UItemHelper::GetItemInfoById(GetWorld(), ItemInfo.GetId());
	
	// 핫바에서 먼저 검색함.
	bool bHasInventory = false;
	// 맨 처음 값은 현재 주운 아이템의 갯수로 지정한다.
	uint32 RemainResult = ItemInfo.GetCurrentCount();
	
	for (int i = 0; i < GetTotalSlotCount(); i++)
	{
		if (PlayerInventoryList[i].GetId() == ItemInfo.GetId()
			&& PlayerInventoryList[i].GetCurrentCount() < ItemInfoById.GetMaxItemCount())
		{
			bHasInventory = true;
			RemainResult = AddItemToInventory(i, ItemInfo);
			break;
		}
	}

	if (!bHasInventory && RemainResult > 0)
	{
		for (int i = 0; i < GetTotalSlotCount(); i++)
		{
			if (PlayerInventoryList[i].GetId() == 0)
			{
				RemainResult = AddItemToInventory(i, ItemInfo);
				break;
			}
		}
	}

	if (RemainResult > 0)
	{
		// TODO: 이후에 대한 처리 로직은 다른 곳에 이관해야 함
		UE_LOG(LogTemp, Error, TEXT("인벤토리 초과함"))
	}

	// UI 업데이트
	ABasePlayerController* PC = static_cast<ABasePlayerController*>(GetPlayerController());

	const UPlayerGameUI* GameUI = static_cast<UPlayerGameUI*>(PC->GetPlayerUI());
	GameUI->GetInventoryHotSlot()->UpdateInventoryArray();
}

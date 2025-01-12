#include "BasePlayerState.h"
#include "project_02/DataTable/ItemInfoData.h"

ABasePlayerState::ABasePlayerState()
{
	
}

void ABasePlayerState::LoadTestPlayerData()
{
	// TODO: 제거해야 할 테스트 코드
	FItemMetaInfo NewItem;
	NewItem.SetId(1);
	NewItem.SetCurrentCount(1);

	PlayerHotSlotList.Add(NewItem);
}

void ABasePlayerState::AddItem(const FItemMetaInfo& ItemInfo)
{
	// 핫바에서 먼저 검색함.
	const int32 ItemToIndex = PlayerHotSlotList.Find(ItemInfo);

	if (ItemToIndex != INDEX_NONE)
	{
		// TODO: 최대 수 일 때 새롭게 넣는 것도 고려해야함.
		PlayerHotSlotList[ItemToIndex].SetCurrentCount(
		PlayerHotSlotList[ItemToIndex].GetCurrentCount() +
			ItemInfo.GetCurrentCount()
		);
	} else
	{
		// TODO: 추후 인벤토리도 검사하고 넣기
		// 넣기 전에 검사할 것

		// TODO: 지금은 완전탐색이지만 Map 자료구조를 이용해
		// 현재 위치에 대한 정보를 기록해두는 것도 좋아보인다.
		for (int i = 0; i < HotSlotCount; i++)
		{
			// 현재 핫슬롯 자체가 비어있는 상태인 경우 아에 추가한다.
			if (PlayerHotSlotList.Num() <= i)
			{
				PlayerHotSlotList.Add(ItemInfo);
				return;
			}

			// 단순히 i번째에 값이 없는 경우에 대해서는 값을 넣어준다.
			if (!PlayerHotSlotList.IsValidIndex(i))
			{
				PlayerHotSlotList[i] = ItemInfo;
			}
		}
	}
}

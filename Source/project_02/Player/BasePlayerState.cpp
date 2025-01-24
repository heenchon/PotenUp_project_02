#include "BasePlayerState.h"

#include "BasePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/InventoryComponent.h"
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
	
	for (int i = 0; i < InitialItemList.Num(); i++)
	{
		FItemMetaInfo NewItem;
		NewItem.SetId(InitialItemList[i]);
		NewItem.SetCurrentCount(FItemHelper::GetItemInfoById(
			GetWorld(), InitialItemList[i])
			.GetMaxItemCount());

		PlayerInventoryList[i] = NewItem;
	}
	
	UpdateCurrentRemainItemValue();
}

// TODO: 이건 내부 로직에서 아이템 변경될 때 마다 처리하기
void ABasePlayerState::UpdateCurrentRemainItemValue()
{
	TMap<uint32, uint32> NewMap;
	
	for (int i = 0; i < InitialItemList.Num(); i++)
	{
		if (NewMap.Find(PlayerInventoryList[i].GetId())) 
		{
			NewMap[PlayerInventoryList[i].GetId()] += PlayerInventoryList[i].GetCurrentCount();
		}
		else
		{
			NewMap.Add(PlayerInventoryList[i].GetId(), PlayerInventoryList[i].GetCurrentCount());
		}
	}

	CurrentRemainItemValue.Append(NewMap);
}

void ABasePlayerState::BeginPlay()
{
}

void ABasePlayerState::SwapItemInInventory(const uint16 Prev, const uint16 Next)
{
	const FItemMetaInfo Temp = PlayerInventoryList[Prev];
	PlayerInventoryList[Prev] = PlayerInventoryList[Next];
	PlayerInventoryList[Next] = Temp;
	
	SetPlayerHandItemByPS(Prev);
	SetPlayerHandItemByPS(Next);
}

// TODO: 해당 함수는 리팩토링이 필요하다. 위치에 맞는 역할은 아니다.
// PlayerController로 옮길 생각 해야함.
void ABasePlayerState::SetPlayerHandItemByPS(const uint16 NewIndex)
{
	// 핫슬롯을 넘은 경우는 처리할 필요 없다.
	if (NewIndex > GetHotSlotCount())
	{
		return;
	}
	
	const FItemInfoData& ItemInfoById = FItemHelper::GetItemInfoById(GetWorld(), PlayerInventoryList[NewIndex].GetId());
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn()))
	{
		if (Player->GetInventoryComponent()->GetSelectedHotSlotIndex() == NewIndex)
		{
			Player->SetViewItemOnHand(ItemInfoById);
		}
	}
}


uint32 ABasePlayerState::AddItemToInventory(const uint16 Index, const FItemMetaInfo& ItemInfo)
{
	const FItemInfoData& ItemInfoById = FItemHelper::GetItemInfoById(GetWorld(), ItemInfo.GetId());

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
	
	// 내가 인벤토리에 넣었을 때 핫슬롯인 경우 그리고 내가 현재 선택하고 있는
	// 핫슬롯인 경우에 액터가 소환되게 처리함.
	SetPlayerHandItemByPS(Index);

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

	UpdateCurrentRemainItemValue();
	return RemainCount > 0 ? RemainCount : 0;
}

bool ABasePlayerState::DropItem(const uint16 Index, const uint32 Count)
{
	// -1은 강제로 다 버리기
	if (Count == -1)
	{
		const FItemMetaInfo ClearItemMeta;
		PlayerInventoryList[Index] = ClearItemMeta;
		OnUpdateInventory();
		UpdateCurrentRemainItemValue();
		return true;
	}
	
	if (PlayerInventoryList[Index].GetCurrentCount() - Count < 0)
	{
		return false;
	}
	else if (PlayerInventoryList[Index].GetCurrentCount() - Count == 0)
	{
		const FItemMetaInfo ClearItemMeta;
		PlayerInventoryList[Index] = ClearItemMeta;
	} else
	{
		PlayerInventoryList[Index].SetCurrentCount(PlayerInventoryList[Index].GetCurrentCount() - Count);
	}
	
	OnUpdateInventory();
	
	return true;
}

uint32 ABasePlayerState::AddItem(const FItemMetaInfo& ItemInfo)
{
	const FItemInfoData& ItemInfoById = FItemHelper::GetItemInfoById(GetWorld(), ItemInfo.GetId());
	
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
		return RemainResult;
	}

	// UI 업데이트
	OnUpdateInventory();
	
	return 0;
}

bool ABasePlayerState::RemoveItem(const uint16 Id, const uint32 Count)
{
	uint32 RemainNum = Count;
	TArray<uint32> CanRemoveIndexList;
	for (int i = 0; i < GetTotalSlotCount(); i++)
	{
		if (PlayerInventoryList[i].GetId() == Id)
		{
			CanRemoveIndexList.Add(i);
			RemainNum = UKismetMathLibrary::Max(RemainNum - PlayerInventoryList[i].GetCurrentCount(), 0);
		}

		// RemainNum이 0보다 작거나 같다는 의미는 즉
		// 더이상 탐색하지 않아도 전부 없앨 수 있다라는 의미다.
		if (RemainNum <= 0)
		{
			break;
		}
	}

	// 0보다 큰 상황이라면 사용하기에는 양이 부족하다라는 의미기에 false를 반환한다.
	if (RemainNum > 0)
	{
		return false;
	}
	
	// 다시 RemainNum을 돌려둔다. 재사용해서 갯수를 없애는 것에 실질적으로 이용함
	RemainNum = Count;
	for (const uint32 RemoveIndex : CanRemoveIndexList)
	{
		// 둘 중 더 작은 값을 없애준다. 만약 RemainNum이 10이고, 아이템의 현재 갯수가 7이라면
		// 7개만 없애주고, 아이템 갯수가 13개면 10개만 없앤다.
		const uint32 RemoveCount = UKismetMathLibrary::Min(PlayerInventoryList[RemoveIndex].GetCurrentCount(), RemainNum);

		// 우선은 값 설정
		PlayerInventoryList[RemoveIndex].SetCurrentCount(PlayerInventoryList[RemoveIndex].GetCurrentCount() - RemoveCount);
		RemainNum -= RemoveCount;
		
		// 만약 슬롯이 현재 0개라면 슬롯 초기화를 진행한다.
		if (PlayerInventoryList[RemoveIndex].GetCurrentCount() == 0)
		{
			const FItemMetaInfo ClearItemMeta;
			PlayerInventoryList[RemoveIndex] = ClearItemMeta;
		}

		// 어차피 여기서 다 버려서 0이 될 수 밖에 없다.
		if (RemainNum == 0)
		{
			OnUpdateInventory();
			return true;
		}
	}

	// 이건 혹시 모르는 예외 사항에 대한 처리
	return false;
}

void ABasePlayerState::OnUpdateInventory()
{
	ABasePlayerController* PC = static_cast<ABasePlayerController*>(GetPlayerController());

	UpdateCurrentRemainItemValue();
	PC->GetPlayerUI()->GetInventoryHotSlot()->UpdateInventoryArray();
}

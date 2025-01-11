// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Game/BaseGameInstance.h"

void UInventorySlot::SetSlotInfo(const FItemMetaInfo& ItemMetaInfo) const
{
	// ID가 0인 경우는 해당 아이템이 존재하지 않는다임을 명시
	if (ItemMetaInfo.GetId() == 0)
	{
		return;
	}
	
	const UBaseGameInstance* GameInstance = static_cast<UBaseGameInstance*>(GetGameInstance());

	UE_LOG(LogTemp, Display, TEXT("테스트 아이템 %d: %d"), ItemMetaInfo.GetId(), ItemMetaInfo.GetCurrentCount());
	ItemThumbnail.Get()->SetBrushFromTexture(
		GameInstance->GetItemInfoList()[ItemMetaInfo.GetId()].GetThumbnail().Get());
	ItemCount->SetText(FText::AsNumber(ItemMetaInfo.GetCurrentCount()));
}

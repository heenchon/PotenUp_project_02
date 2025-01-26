#include "CraftingList.h"

#include "CraftingDetail.h"
#include "Components/ScrollBox.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/Widgets/Crafting/CraftingInfo.h"
#include "project_02/Helper/ItemHelper.h"

void UCraftingList::NativeOnInitialized()
{
	const UBaseGameInstance* GI = GetWorld()->GetGameInstance<UBaseGameInstance>();
	
	for (const TTuple<uint32, FCraftingData> CraftingInfoMap : GI->GetCraftingInfoMap())
	{
		FItemInfoData ItemInfoData = FItemHelper::GetItemInfoById(GetWorld(),
				CraftingInfoMap.Key);
		FText NewName = FText::FromString(ItemInfoData.GetDisplayName());
		
		if (!CraftingInfoClass)
		{
			return;
		}
		
		if (UCraftingInfo* NewCraftingInfo = CreateWidget<UCraftingInfo>(this, CraftingInfoClass))
		{
			NewCraftingInfo->InitializeData(CraftingInfoMap.Key, ItemInfoData);
			NewCraftingInfo->OnClickCraftingInfoById.AddDynamic(this, &ThisClass::UpdateCraftingDetail);
			CraftingList->AddChild(NewCraftingInfo);
		}
	}
}

void UCraftingList::UpdateCraftingDetail(const uint32 ItemId)
{
	CraftingDetail->SetCraftingId(ItemId);
	CraftingDetail->UpdateRequireList();
	CraftingDetail->SetVisibility(ESlateVisibility::Visible);
}


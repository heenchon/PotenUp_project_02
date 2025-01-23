#include "CraftingList.h"

#include "Components/ScrollBox.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/Widgets/Crafting/CraftingInfo.h"
#include "project_02/Helper/ItemHelper.h"

void UCraftingList::NativeConstruct()
{
	const UBaseGameInstance* GI =
		static_cast<UBaseGameInstance*>(GetWorld()->GetGameInstance());

	
	for (const TTuple<uint32, FCraftingData> CraftingInfoMap : GI->GetCraftingInfoMap())
	{
		FItemInfoData ItemInfoData = FItemHelper::GetItemInfoById(GetWorld(),
				static_cast<uint16>(CraftingInfoMap.Key));
		FText NewName = FText::FromString(ItemInfoData.GetDisplayName());
		if (!CraftingInfoClass)
		{
			return;
		}
		if (UCraftingInfo* NewCraftingInfo = CreateWidget<UCraftingInfo>(this, CraftingInfoClass))
		{
			NewCraftingInfo->SetItemName(NewName);
			NewCraftingInfo->SetItemThumbnail(ItemInfoData.GetThumbnail());
			CraftingList->AddChild(NewCraftingInfo);
		}
	}
}

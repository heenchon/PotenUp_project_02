#include "CraftingDetail.h"

#include "Components/VerticalBox.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/Helper/ItemHelper.h"
#include "CraftingDetailInfo.h"

void UCraftingDetail::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UCraftingDetail::UpdateRequireList()
{
	const UBaseGameInstance* GI = GetWorld()->GetGameInstance<UBaseGameInstance>();

	if (!GI)
	{
		return;
	}

	RequireList->ClearChildren();
	
	if (GI->GetCraftingInfoMap().Find(CraftingId))
	{
		const FCraftingData CraftData = GI->GetCraftingInfoMap()[CraftingId];
		
		for (const TPair<uint32, uint32> NewRecipe : CraftData.Recipe.Array())
		{
			FItemHelper::GetItemInfoById(GetWorld(), NewRecipe.Key);
			UCraftingDetailInfo* DetailInfo = CreateWidget<UCraftingDetailInfo>(
				this, CraftingDetailInfoClass
			);
			DetailInfo->InitializeData(NewRecipe);
			RequireList->AddChild(DetailInfo);
		}
	}
}

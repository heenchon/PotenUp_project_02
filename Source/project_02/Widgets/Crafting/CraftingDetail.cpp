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

		if (CraftingDetailInfoClass)
		{
			for (const TPair<uint32, uint32> NewRecipe : CraftData.Recipe.Array())
			{
				UE_LOG(LogTemp, Display, TEXT("테스트2 :%d, %d"), NewRecipe.Key, NewRecipe.Value);
				
				FItemHelper::GetItemInfoById(GetWorld(), NewRecipe.Key);
				UCraftingDetailInfo* DetailInfo = CreateWidget<UCraftingDetailInfo>(
					this, CraftingDetailInfoClass
				);
				
				RequireList->AddChild(DetailInfo);
			}
		}
	}
}

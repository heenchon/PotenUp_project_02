#include "CraftingDetail.h"

#include "Components/VerticalBox.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/Helper/ItemHelper.h"
#include "CraftingDetailInfo.h"
#include "Components/Button.h"
#include "project_02/Player/BasePlayerState.h"

void UCraftingDetail::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);
	ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::OnClickConfirmButton);
}

void UCraftingDetail::InitializeRequireList()
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

void UCraftingDetail::OnClickConfirmButton()
{
	const UBaseGameInstance* GI = GetWorld()->GetGameInstance<UBaseGameInstance>();

	if (!GI)
	{
		return;
	}
	
	ABasePlayerState* PS = GetOwningPlayerState<ABasePlayerState>();
	if (!PS)
	{
		return;
	}

	if (GI->GetCraftingInfoMap().Find(CraftingId))
	{
		const FCraftingData CraftData = GI->GetCraftingInfoMap()[CraftingId];
		// 아이템 검증 과정
		for (const TPair<uint32, uint32> NewRecipe : CraftData.Recipe.Array())
		{
			// 하나라도 아이템이 존재하지 않는 경우에는 더이상 처리하지 않는다.
			if (!PS->HasItemInInventory(NewRecipe.Key, NewRecipe.Value))
			{
				return;
			}
		}
		
		// 위에서 검증이 되었기 때문에 아이템은 우선 추가해둔다.
		// 만약 추가에 실패한 경우는 그냥 무시처리 한다. 인벤토리에서 추가에 실패하면 아이템을 넣지 않기 때문이다.
		// TODO: 남은 값에 대해서는 가져와주기 때문에 추후 아이템 드롭이 만들어진다면 그만큼 던져주면 된다.
		if (PS->AddItem(FItemHelper::GetInitialItemMetaDataById(GetWorld(), CraftingId)) == 0)
		{
			// 아래서 검증이 완료되었기 때문에 여기서 제거 처리
			for (const TPair<uint32, uint32> NewRecipe : CraftData.Recipe.Array())
			{
				PS->RemoveItem(NewRecipe.Key, NewRecipe.Value);
			}

			// TODO: 이거는 지우고 다시 작성하는 방식이기 때문에 숫자만 바꾸는 방식을 고려해야함.
			InitializeRequireList();
		}
	}
}

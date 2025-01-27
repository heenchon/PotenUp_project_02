#include "CraftingDetailInfo.h"

#include "Components/TextBlock.h"
#include "project_02/Helper/ItemHelper.h"
#include "project_02/Player/BasePlayerState.h"

void UCraftingDetailInfo::InitializeData(
	const TPair<uint32, uint32>& CraftItemKeyAndCount) const
{
	const ABasePlayerState* PS = GetOwningPlayerPawn()->GetPlayerState<ABasePlayerState>();
	if (!PS)
	{
		return;
	}

	const FText ItemName =
		FText::FromString(
			FItemHelper::GetItemInfoById(GetWorld(), CraftItemKeyAndCount.Key).GetDisplayName());
	ItemNameText->SetText(ItemName);

	const uint32 RemainItemCount =
		PS->GetCurrentRemainItemValue().Find(CraftItemKeyAndCount.Key) ?
			PS->GetCurrentRemainItemValue()[CraftItemKeyAndCount.Key] : 0;
	
	const FText ItemRemainCountText = FText::FromString(
		FString::Printf(TEXT("%d"), RemainItemCount));
	ItemCountText->SetText(ItemRemainCountText);
	
	const FText ItemNeedCountText = FText::FromString(
		FString::Printf(TEXT("%d"), CraftItemKeyAndCount.Value));
	ItemMaxCountText->SetText(ItemNeedCountText);
}

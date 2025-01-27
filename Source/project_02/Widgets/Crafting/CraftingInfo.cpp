#include "CraftingInfo.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "project_02/DataTable/ItemInfoData.h"

void UCraftingInfo::NativeConstruct()
{
	InfoButton->OnClicked.AddDynamic(this, &ThisClass::OnButtonClick);
}

void UCraftingInfo::InitializeData(const uint32 Id, const FItemInfoData& ItemInfoData)
{
	SetId(Id);
	SetItemName(FText::FromString(ItemInfoData.GetDisplayName()));
	SetItemThumbnail(ItemInfoData.GetThumbnail());
}

void UCraftingInfo::SetItemThumbnail(const TSoftObjectPtr<UTexture2D>& Thumbnail)
{
	UTexture2D* LoadedTexture = Thumbnail.LoadSynchronous();
	ItemThumbnail.Get()->SetBrushFromTexture(LoadedTexture);
}

void UCraftingInfo::OnButtonClick()
{
	OnClickCraftingInfoById.Broadcast(ItemId);
}

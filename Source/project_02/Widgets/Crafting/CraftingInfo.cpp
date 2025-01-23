#include "CraftingInfo.h"

#include "Components/Image.h"

void UCraftingInfo::SetItemThumbnail(const TSoftObjectPtr<UTexture2D>& Thumbnail)
{
	UTexture2D* LoadedTexture = Thumbnail.LoadSynchronous();
	ItemThumbnail.Get()->SetBrushFromTexture(LoadedTexture);
}

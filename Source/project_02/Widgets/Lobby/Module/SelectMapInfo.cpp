#include "SelectMapInfo.h"

#include "Components/TextBlock.h"

void USelectMapInfo::SetMapName(const FString& NewMapName) const
{
	MapName->SetText(FText::FromString(NewMapName));
}

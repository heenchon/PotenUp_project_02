#include "SelectMapList.h"

#include "SelectMapInfo.h"
#include "Components/VerticalBox.h"
#include "project_02/Game/BaseGameInstance.h"

void USelectMapList::NativeConstruct()
{
	const UBaseGameInstance* GI = GetWorld()->GetGameInstance<UBaseGameInstance>();

	if (!GI)
	{
		return;
	}

	for (const FSaveData& SaveGameData : GI->GetSaveDataList())
	{
		USelectMapInfo* MapInfo = CreateWidget<USelectMapInfo>(
			this, SelectInfoClass);
		MapInfo->SetMapName(SaveGameData.MapName);

		RequireList->AddChild(MapInfo);
	}
}
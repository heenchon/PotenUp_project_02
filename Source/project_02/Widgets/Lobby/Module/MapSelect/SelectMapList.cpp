#include "SelectMapList.h"

#include "SelectMapInfo.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Game/RaftSaveList.h"

void USelectMapList::NativeConstruct()
{
	const URaftSaveList* SaveData = Cast<URaftSaveList>(
				UGameplayStatics::LoadGameFromSlot("SaveList", 0));

	if (!SaveData)
	{
		return;
	}

	for (const FSaveData& SaveGameData : SaveData->MapNameList)
	{
		USelectMapInfo* MapInfo = CreateWidget<USelectMapInfo>(
			this, SelectInfoClass);
		MapInfo->SetMapName(SaveGameData.MapName);

		RequireList->AddChild(MapInfo);
	}
}

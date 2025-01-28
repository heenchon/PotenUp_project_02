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

	for (const FString& SaveGameName : GI->GetSaveNameList())
	{
		USelectMapInfo* MapInfo = CreateWidget<USelectMapInfo>(
			this, SelectInfoClass);
		MapInfo->SetMapName(SaveGameName);

		RequireList->AddChild(MapInfo);
	}
}
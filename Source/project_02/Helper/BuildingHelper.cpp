#include "BuildingHelper.h"

#include "EnumHelper.h"
#include "project_02/Game/BaseGameInstance.h"

TSubclassOf<ABuildingActor> FBuildingHelper::GetBuildingClass(
	const UWorld* World, const EBlockType BlockType, const EBlockCategory BlockCategory)
{
	const UBaseGameInstance* GI = World->GetGameInstance<UBaseGameInstance>();

	if (!GI)
	{
		return nullptr;
	}

	const FString Key = FString::Printf(TEXT("%s_%s"),
		*FEnumHelper::GetClassEnumKeyAsString(BlockType),
		*FEnumHelper::GetClassEnumKeyAsString(BlockCategory));
	
	return GI->GetBuildingInfoMap().FindRef(Key);
}
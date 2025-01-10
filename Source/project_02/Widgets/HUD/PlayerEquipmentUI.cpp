#include "PlayerEquipmentUI.h"

#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Game/BaseGameInstance.h"

void UPlayerEquipmentUI::NativeOnInitialized()
{
	// 어차피 GameInstance는 이것만 쓸 것 이다.
	if (const UBaseGameInstance* BaseGameInstance = static_cast<UBaseGameInstance*>(GetGameInstance()))
	{
		for (FItemInfoData& ItemInfo : BaseGameInstance->GetItemInfoList())
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *ItemInfo.GetDisplayName());
		}
	}
}

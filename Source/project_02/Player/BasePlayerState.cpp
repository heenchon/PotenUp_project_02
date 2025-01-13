#include "BasePlayerState.h"
#include "project_02/DataTable/ItemInfoData.h"

ABasePlayerState::ABasePlayerState()
{
	
}

void ABasePlayerState::LoadTestPlayerData()
{
	// TODO: 제거해야 할 테스트 코드
	FItemMetaInfo NewItem;
	NewItem.SetId(1);
	NewItem.SetCurrentCount(1);

	PlayerHotSlotList.Add(NewItem);
}

void ABasePlayerState::BeginPlay()
{
	
}

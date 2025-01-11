#include "BasePlayerState.h"
#include "project_02/DataTable/ItemInfoData.h"

ABasePlayerState::ABasePlayerState()
{
	
}

void ABasePlayerState::BeginPlay()
{
	// 테스트 코드
	// TODO: 제거해야 할 코드
	FItemMetaInfo NewItem;
	NewItem.SetId(1);
	NewItem.SetCurrentCount(10);

	PlayerInventoryList.Add(NewItem);

	FItemMetaInfo NewItem1;
	NewItem1.SetId(2);
	NewItem1.SetCurrentCount(3);

	PlayerInventoryList.Add(NewItem1);

	FItemMetaInfo NewItem2;
	NewItem2.SetId(3);
	NewItem2.SetCurrentCount(2);

	PlayerInventoryList.Add(NewItem2);
}

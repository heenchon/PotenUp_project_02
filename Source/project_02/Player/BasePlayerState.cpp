#include "BasePlayerState.h"

ABasePlayerState::ABasePlayerState()
{
	
}

// 무조건 true로 return 받지만 혹시 모를 딜레이 문제로 인해 아이템을
// 여러개 먹는 것을 방지하기 위해 bool을 return 받아 이 함수가 끝날 때
// 까지 기다리게 하는 것이 목적이다.
// bool ABasePlayerState::AddItem(const FItemMetaInfo& NewItem)
// {
//
// 	// for (FItemMetaInfo& InventoryList : PlayerInventoryList)
// 	// {
// 	// 	
// 	// }
// 	
// 	return true;
// }


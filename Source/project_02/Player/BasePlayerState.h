#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"


struct FItemMetaInfo;

UCLASS()
class PROJECT_02_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABasePlayerState();

	void LoadTestPlayerData();
	
	FORCEINLINE uint8 GetHotSlotCount() const  { return HotSlotCount; }
	FORCEINLINE uint8 GetInventorySlotCount() const  { return InventorySlotCount; }
	FORCEINLINE TArray<FItemMetaInfo> GetPlayerInventoryList() const { return PlayerInventoryList; }
	FORCEINLINE TArray<FItemMetaInfo> GetPlayerHotSlotList() const { return PlayerHotSlotList; }

	void AddItem(const FItemMetaInfo& ItemInfo);
	
private:
	UPROPERTY()
	TArray<FItemMetaInfo> PlayerInventoryList;
	
	UPROPERTY()
	TArray<FItemMetaInfo> PlayerHotSlotList;

	// TODO: 아래와 같은 설정은 추후 Data Asset으로 이전해보기
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 InventorySlotCount = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 HotSlotCount = 0;
};

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

	FORCEINLINE uint8 GetInventorySlotCount() const  { return InventorySlotCount; }
	FORCEINLINE TArray<FItemMetaInfo> GetPlayerInventoryList() const { return PlayerInventoryList; }

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TArray<FItemMetaInfo> PlayerInventoryList;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 InventorySlotCount = 0;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

// struct FItemMetaInfo;

UCLASS()
class PROJECT_02_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABasePlayerState();

	// bool AddItem(const FItemMetaInfo& NewItem);
	
private:
	// TArray<FItemMetaInfo> PlayerInventoryList;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 MaxInventorySlotCount = 0;
};

#pragma once

#include "CoreMinimal.h"
#include "RaftSaveList.h"
#include "Engine/GameInstance.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "BaseGameInstance.generated.h"

UCLASS()
class PROJECT_02_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UBaseGameInstance();

public:
	FORCEINLINE TArray<FSaveData> GetSaveDataList() const{ return SaveDataList; }
	FORCEINLINE TArray<FItemInfoData> GetItemInfoList() const{ return ItemInfoList; }
	FORCEINLINE TMap<uint32, FCraftingData> GetCraftingInfoMap() const{ return CraftingInfoMap; }

protected:
	virtual void Init() override;
	
private:
	UPROPERTY()
	TArray<FItemInfoData> ItemInfoList;
	
	UPROPERTY()
	TArray<FSaveData> SaveDataList;
	
	UPROPERTY()
	TMap<uint32, FCraftingData> CraftingInfoMap;
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/DataTable/CreditData.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "BaseGameInstance.generated.h"

class ABuildingActor;

UCLASS()
class PROJECT_02_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UBaseGameInstance();

public:
	FORCEINLINE TArray<FItemInfoData> GetItemInfoList() const{ return ItemInfoList; }
	FORCEINLINE TMap<uint32, FCraftingData> GetCraftingInfoMap() const{ return CraftingInfoMap; }
	FORCEINLINE TMap<FString, TSubclassOf<ABuildingActor>> GetBuildingInfoMap() const{ return BuildingInfoMap; }
	FORCEINLINE TArray<FCreditData> GetCreditDataList() const{ return CreditDataList; }
	
protected:
	virtual void Init() override;
	
private:
	UPROPERTY()
	TArray<FItemInfoData> ItemInfoList;
	
	UPROPERTY()
	TMap<uint32, FCraftingData> CraftingInfoMap;

	UPROPERTY()
	TMap<FString, TSubclassOf<ABuildingActor>> BuildingInfoMap;
	
	UPROPERTY()
	TArray<FCreditData> CreditDataList;
	
	void LoadItemData();

	void LoadCraftingData();

	void LoadBuildingData();

	void LoadCreditData();
};

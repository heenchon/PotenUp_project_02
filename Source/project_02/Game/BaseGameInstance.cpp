#include "BaseGameInstance.h"

#include "RaftSaveList.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/DataTable/ItemInfoData.h"

UBaseGameInstance::UBaseGameInstance()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		ItemInfoDataTable(TEXT("/Script/Engine.DataTable'/Game/Sangmin/DataTable/DT_ItemData.DT_ItemData'"));

	if (ItemInfoDataTable.Succeeded())
	{
		TArray<FItemInfoData*> TempItemInfoList;
		ItemInfoDataTable.Object->GetAllRows<FItemInfoData>(TEXT(""), TempItemInfoList);
		
		for (const FItemInfoData* InfoItem : TempItemInfoList)
		{
			ItemInfoList.Add(*InfoItem);
		}
	}

	const static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingInfoDataTable(TEXT("/Script/Engine.DataTable'/Game/Sangmin/DataTable/DT_CraftingData.DT_CraftingData'"));
	
	if (CraftingInfoDataTable.Succeeded())
	{
		TArray<FName> RowNames = CraftingInfoDataTable.Object->GetRowNames();
		
		for (int i = 0; i < RowNames.Num(); i++)
		{
			const FCraftingData* Data = CraftingInfoDataTable.Object->FindRow<FCraftingData>(RowNames[i], "");
			CraftingInfoMap.Add(FCString::Atoi(*RowNames[i].ToString()), *Data);
		}
	}
}

void UBaseGameInstance::Init()
{
	if (const URaftSaveList* RaftSaveList = Cast<URaftSaveList>(
		UGameplayStatics::LoadGameFromSlot("SaveList", 0)))
	{
		SaveDataList = RaftSaveList->MapNameList;
	}
}
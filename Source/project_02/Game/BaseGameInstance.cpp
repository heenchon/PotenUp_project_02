#include "BaseGameInstance.h"

#include "project_02/DataTable/CraftingData.h"
#include "project_02/DataTable/ItemInfoData.h"

UBaseGameInstance::UBaseGameInstance()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		ItemInfoDataTable(TEXT("/Script/Engine.DataTable'/Game/Sangmin/DataTable/DT_ItemData.DT_ItemData'"));

	if (ItemInfoDataTable.Succeeded())
	{
		TArray<FItemInfoData*> TempInfoList;
		ItemInfoDataTable.Object->GetAllRows<FItemInfoData>(TEXT(""), TempInfoList);
		
		for (const FItemInfoData* InfoItem : TempInfoList)
		{
			ItemInfoList.Add(*InfoItem);
		}
	}

	const static ConstructorHelpers::FObjectFinder<UDataTable>
		CraftingInfoDataTable(TEXT("/Script/Engine.DataTable'/Game/Sangmin/DataTable/DT_CraftingData.DT_CraftingData'"));
	
	if (CraftingInfoDataTable.Succeeded())
	{
		TArray<FCraftingData*> TempInfoList;
		CraftingInfoDataTable.Object->GetAllRows<FCraftingData>(TEXT(""), TempInfoList);
		TArray<FName> RowNames = CraftingInfoDataTable.Object->GetRowNames();
		
		for (int i = 0; i < TempInfoList.Num(); i++)
		{
			CraftingInfoMap.Add(FCString::Atoi(*RowNames[i].ToString()), *TempInfoList[i]);
		}
	}
}

void UBaseGameInstance::Init()
{
	
}

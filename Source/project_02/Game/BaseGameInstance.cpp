#include "BaseGameInstance.h"

#include "project_02/DataTable/BuildData.h"
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

	const static ConstructorHelpers::FObjectFinder<UDataTable>
		BuildingInfoDataTable(TEXT("/Script/Engine.DataTable'/Game/Sangmin/DataTable/DT_BuildingInfo.DT_BuildingInfo'"));

	if (BuildingInfoDataTable.Succeeded())
	{
		TArray<FName> RowNames = BuildingInfoDataTable.Object->GetRowNames();
		
		for (int i = 0; i < RowNames.Num(); i++)
		{
			const FBuildingInfo* Data = BuildingInfoDataTable.Object->FindRow<FBuildingInfo>(RowNames[i], "");
			BuildingInfoMap.Add(RowNames[i].ToString(), Data->GetBuildClass());
		}
	}
}

void UBaseGameInstance::Init()
{
}
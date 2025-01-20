#include "BaseGameInstance.h"

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
}

void UBaseGameInstance::Init()
{
	
}

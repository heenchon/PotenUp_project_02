#include "BaseGameInstance.h"
#include "project_02/DataTable/BuildData.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/DataTable/CreditData.h"
#include "project_02/DataTable/ItemInfoData.h"

UBaseGameInstance::UBaseGameInstance()
{
	LoadItemData();
	LoadCraftingData();
	LoadBuildingData();
	LoadCreditData();
}

void UBaseGameInstance::Init()
{
}

void UBaseGameInstance::LoadItemData()
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
}

void UBaseGameInstance::LoadCraftingData()
{
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

void UBaseGameInstance::LoadBuildingData()
{
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

// TODO: 이거 Helper 함수로 옮겨도 되지 않을까 싶음...
void UBaseGameInstance::LoadCreditData()
{
	const FString ProjectDir = FPaths::ProjectDir();
	const FString FilePath = FPaths::Combine(ProjectDir, TEXT("history.csv"));
	
	if (!FPaths::FileExists(FilePath))
	{
		return;
	}

	TArray<FString> Contents;
	const bool IsLoadSuccess = FFileHelper::LoadFileToStringArray(Contents, *FilePath);

	if (!IsLoadSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Fail To Load File: %d"), FPlatformMisc::GetLastError());
		return;
	}

	uint32 NewIndex = 0;
	for (FString Content : Contents)
	{
		FString Temp;
		TArray<FString> Value;
		bool IsStart = false;

		// 한 줄의 데이터를 하나의 글자를 계속해서 비교한다.
		for (int i = 0; i < Content.Len(); i++)
		{
			// 현재 시작하지 않은 즉 데이터 초기화 후 가져오는 시점에서 호출됨
			if (!IsStart)
			{
				// 해당 문자열의 시작이 " 로 시작하는가를 봐야한다.
				// csv 데이터에서 하나의 column 시작은 반드시 "로 된다.
				if (Content[i] == '"')
				{
					IsStart = true;
				}
			} else
			{
				// 시작된 데이터의 상태에는 이제 "가 나오지 않는 이상은 데이터를
				// 계속 넣어주면 되고 "가 나오는 경우에만 처리해준다.
				if (Content[i] == '"') {
					// "가 나온 시점에서 다음 번째가 Content의 Size의 숫자라는 뜻은
					// 해당 시점은 마지막 글자라는 것이기 때문에 계속 넣어주던 정보를 다 넣어주고 그냥 마무리 한다.
					if (i + 1 == Content.Len())
					{
						Value.Add(Temp);
						Temp = "";
						// 말이 break지 사실은 이미 반복문이 끝난 상태이다.
						break;
					}
					// " 다음에 , 가 나온다면 문자열의 끝임을 명시한다.
					// Git Log는 직접 커스텀 로그로 만들어야 해서 문제가 생길 수 있지만
					// 실제 csv로 parsing된 데이터는 빵"가","루 -> "빵""가"",""루"
					// 즉 "," 는 -> "","" 로 나올 수 있다.
					// 이런 부분의 예외처리를 원래는 다르게 해줘야 하지만 Git Log은 일반적인
					// csv 형태로 parsing되는 것은 아니기 때문에 쉽게 처리한다.
					if (Content[i + 1] == ',')
					{
						IsStart = false;
						Value.Add(Temp);
						Temp = "";
					} else
					{
						Temp += Content[i];
					}
				} else
				{
					Temp += Content[i];
				}
			}
		}

		FCreditData NewCreditData;
		NewCreditData.Index = ++NewIndex;
		
		NewCreditData.DeveloperName = Value[0];
		
		FDateTime CreditDateTime;
		FDateTime::Parse(Value[1], CreditDateTime);
		NewCreditData.CommitDate = CreditDateTime;
		
		NewCreditData.CommitDesc = Value[2];

		CreditDataList.Add(NewCreditData);
	}
}

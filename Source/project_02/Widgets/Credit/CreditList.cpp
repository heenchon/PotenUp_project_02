#include "CreditList.h"

#include "CreditObject.h"
#include "Components/ListView.h"
#include "project_02/Game/BaseGameInstance.h"

void UCreditList::NativeConstruct()
{
	Super::NativeConstruct();

	CreditList->SetScrollbarVisibility(ESlateVisibility::Hidden);

	for (int i = 0; i < DataCountEmpty; i++)
	{
		UCreditObject* CreditObject = NewObject<UCreditObject>();
		CreditList->AddItem(CreditObject);
		ScrollHeightToGetNextData += ItemHeight;
	}
	
	AddDataToScroll();
}

void UCreditList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float CurrentScroll = CreditList->GetScrollOffset();

	CreditList->SetScrollOffset(CurrentScroll + ScrollSpeed * InDeltaTime);

	UE_LOG(LogTemp, Display, TEXT("%d"), CreditList->GetListItems().Num());
	if (!IsEndScroll && CreditList->GetScrollOffset() >= ScrollHeightToGetNextData)
	{
		AddDataToScroll();
	}
}

void UCreditList::AddDataToScroll()
{
	if (IsLoading)
	{
		return;
	}
	IsLoading = true;
	
	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	
	const int MaxValue = FMath::Min(CurrentIndex + DataCountPerScroll, GI->GetCreditDataList().Num());
	// 가져올 데이터보다 적게 가져와야 한다면 스크롤의 끝임을 알린다.
	IsEndScroll = MaxValue < DataCountPerScroll;
	for (int i = CurrentIndex; i < MaxValue; i++)
	{
		UCreditObject* CreditObject = NewObject<UCreditObject>();
		CreditObject->Data = GI->GetCreditDataList()[i];
		CreditList->AddItem(CreditObject);
	}

	if (!IsEndScroll)
	{
		ScrollHeightToGetNextData += IndexToFindNext * ItemHeight;
	}
	
	CurrentIndex = MaxValue;
	
	IsLoading = false;
}

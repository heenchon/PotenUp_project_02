#include "CreditList.h"

#include "CreditObject.h"
#include "Components/ListView.h"
#include "project_02/Game/BaseGameInstance.h"

void UCreditList::NativeConstruct()
{
	Super::NativeConstruct();

	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();

	CreditList->SetScrollbarVisibility(ESlateVisibility::Hidden);
	CreditList->OnItemScrolledIntoView().AddUFunction(this, TEXT("OnScrollMoveItem"));
	
	for (int i = 0; i < GI->GetCreditDataList().Num(); i++)
	{
		UCreditObject* CreditObject = NewObject<UCreditObject>();
		CreditObject->Data = GI->GetCreditDataList()[i];
		CreditList->AddItem(CreditObject);
	}
}

void UCreditList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float CurrentScroll = CreditList->GetScrollOffset();

	CreditList->SetScrollOffset(CurrentScroll + ScrollSpeed * InDeltaTime);
}

void UCreditList::OnScrollMoveItem(UObject* Item, UUserWidget& Widget)
{
	const UCreditObject* CreditData = Cast<UCreditObject>(Item);
	if (!CreditData)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Scroll: %d"), CreditData->Data.Index);
}
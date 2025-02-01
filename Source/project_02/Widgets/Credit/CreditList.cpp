#include "CreditList.h"

#include "CreditObject.h"
#include "Components/ListView.h"
#include "project_02/Game/BaseGameInstance.h"

void UCreditList::NativeConstruct()
{
	// CreditList->OnItemScrolledIntoView().AddUFunction(
	// 	this, "OnScrollEnd");

	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	
	for (int i = 0; i < GI->GetCreditDataList().Num(); i++)
	{
		UCreditObject* CreditObject = NewObject<UCreditObject>();
		CreditObject->Data = GI->GetCreditDataList()[i];
		CreditList->AddItem(CreditObject);
	}
}

// void UCreditList::OnScrollEnd(UObject* Item, UUserWidget* Widget)
// {
// 	const UCreditObject* CreditObject = Cast<UCreditObject>(Item);
// 	if (!CreditObject)
// 	{
// 		return;
// 	}
// 	
// 	UE_LOG(LogTemp, Display, TEXT("%s"), *CreditObject->Data.DeveloperName);
// }

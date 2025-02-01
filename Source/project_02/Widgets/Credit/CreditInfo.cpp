#include "CreditInfo.h"

#include "CreditObject.h"
#include "Components/TextBlock.h"

void UCreditInfo::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	const UCreditObject* CreditObject = Cast<UCreditObject>(ListItemObject);

	if (!CreditObject)
	{
		return;
	}

	Title->SetText(FText::FromString(CreditObject->Data.DeveloperName));
	DateTime->SetText(FText::FromString(CreditObject->Data.CommitDate.ToFormattedString(TEXT("yyyy.mm.dd"))));
	Desc->SetText(FText::FromString(CreditObject->Data.CommitDesc));
}

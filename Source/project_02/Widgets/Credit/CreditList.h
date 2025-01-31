#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditList.generated.h"

class UListView;

UCLASS()
class PROJECT_02_API UCreditList : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> CreditList;
};
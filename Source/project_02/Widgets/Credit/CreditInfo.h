#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "CreditInfo.generated.h"

class UTextBlock;

UCLASS()
class PROJECT_02_API UCreditInfo : public UUserWidget
	, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DateTime;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Desc;
};

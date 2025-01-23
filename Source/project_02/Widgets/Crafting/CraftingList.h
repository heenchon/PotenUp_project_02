#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingList.generated.h"


class UCraftingInfo;
class UScrollBox;

UCLASS()
class PROJECT_02_API UCraftingList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> CraftingList;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCraftingInfo> CraftingInfoClass;
};

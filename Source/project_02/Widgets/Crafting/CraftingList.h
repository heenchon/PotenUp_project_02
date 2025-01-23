#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingList.generated.h"


class UCraftingDetail;
class UCraftingInfo;
class UScrollBox;

UCLASS()
class PROJECT_02_API UCraftingList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> CraftingList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCraftingDetail> CraftingDetail;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCraftingInfo> CraftingInfoClass;
};

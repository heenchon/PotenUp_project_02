#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentUI.generated.h"


class UCraftingList;
class UInventoryList;

UCLASS()
class PROJECT_02_API UPlayerEquipmentUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCraftingList> CraftingList;
};

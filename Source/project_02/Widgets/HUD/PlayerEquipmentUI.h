#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "project_02/Widgets/Crafting/CraftingList.h"
#include "PlayerEquipmentUI.generated.h"


class UCraftingDetail;
class UCraftingList;
class UInventoryList;

UCLASS()
class PROJECT_02_API UPlayerEquipmentUI : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE TObjectPtr<UInventoryList> GetInventoryList() { return InventoryList; }
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCraftingList> CraftingList;
};

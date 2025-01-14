#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryHotSlot.generated.h"

class UGridPanel;
class UInventorySlot;

UCLASS()
class PROJECT_02_API UInventoryHotSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// TODO: 테스트를 위한 코드로 추후 제거할 예정
	void UpdateInventoryArray();
	
	void SetInventoryArray(const uint8 NewCount);
	
	void ChangeSelectedHotSlot(const uint8 PrevSlot, const uint8 NextSlot);

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 SlotCount;

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventorySlot> ItemSlotClass;

	UPROPERTY()
	TArray<TObjectPtr<UInventorySlot>> ItemSlotList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> ItemGridList;
};

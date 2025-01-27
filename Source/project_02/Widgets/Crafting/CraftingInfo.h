#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "CraftingInfo.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickCraftingInfoById, uint32, Id);

UCLASS()
class PROJECT_02_API UCraftingInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemName(const FText& NewItemName) const { ItemName->SetText(NewItemName); }
	FORCEINLINE TObjectPtr<UButton> GetInfoButton() { return InfoButton; }
	void SetItemThumbnail(const TSoftObjectPtr<UTexture2D>& Thumbnail);
	void InitializeData(const uint32 Id, const FItemInfoData& ItemInfoData);
	FORCEINLINE void SetId(const uint32 NewId) { ItemId = NewId; }
	FORCEINLINE uint32 GetId() const { return ItemId; }

	FClickCraftingInfoById OnClickCraftingInfoById;

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InfoButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;

	uint32 ItemId;

	UFUNCTION()
	void OnButtonClick();
};

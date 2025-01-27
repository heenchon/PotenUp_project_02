#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingDetailInfo.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class PROJECT_02_API UCraftingDetailInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeData(const TPair<uint32, uint32>& CraftItemKeyAndCount) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCountText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemMaxCountText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingDetail.generated.h"

class UOverlay;

UCLASS()
class PROJECT_02_API UCraftingDetail : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateRequireList();
	FORCEINLINE void SetCraftingId(const uint32 NewId) { CraftingId = NewId; }

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> RequireList;

	uint32 CraftingId;
};

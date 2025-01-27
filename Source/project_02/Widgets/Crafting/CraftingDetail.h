#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingDetail.generated.h"

class UButton;
class UVerticalBox;
class UCraftingDetailInfo;
class UOverlay;

UCLASS()
class PROJECT_02_API UCraftingDetail : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeRequireList();
	FORCEINLINE void SetCraftingId(const uint32 NewId) { CraftingId = NewId; }

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> RequireList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCraftingDetailInfo> CraftingDetailInfoClass;

	uint32 CraftingId;

	UFUNCTION()
	void OnClickConfirmButton();
};

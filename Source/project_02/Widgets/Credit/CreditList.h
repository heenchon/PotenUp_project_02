#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditList.generated.h"

class UListView;

UCLASS()
class PROJECT_02_API UCreditList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> CreditList;

	// UFUNCTION()
	// void OnScrollEnd(UObject* Item, UUserWidget* Widget);

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 DataCountPerScroll = 10;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float ScrollSpeed = 0.2f;

	uint16 CurrentIndex = 0;
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerGameUI.generated.h"

class UImage;
class UTextBlock;
class UOverlay;
class UInventoryHotSlot;
class UStatusProgressBar;
class UItemUI;

UCLASS()
class PROJECT_02_API UPlayerGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHotSlotIndex(const uint8 PrevIndex, const uint8 NextIndex);
	void SetHealthPercent(const uint8 Current, const uint8 Max);
	void SetHungerPercent(const uint8 Current, const uint8 Max);
	void SetThirstPercent(const uint8 Current, const uint8 Max);

	void SetInteractiveUIStatus(const AActor* Target);
	void SetProgressPercent(const float Percent);

	FORCEINLINE TObjectPtr<UInventoryHotSlot> GetInventoryHotSlot() const { return InventoryHotSlot; }
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusProgressBar> HungerBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusProgressBar> ThirstBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryHotSlot> InventoryHotSlot;

	// TODO: Interactive관련 모듈화 처리 하는 것을 고려해볼 것
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> InteractiveUI;
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractiveUITitle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> HookProgressBar;
	

public:
	//희연
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemUI> ItemMainUI;
};

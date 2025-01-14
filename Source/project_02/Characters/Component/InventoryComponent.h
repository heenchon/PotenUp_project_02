#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInputAction;
class UPlayerEquipmentUI;

struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	FORCEINLINE uint8 GetSelectedHotSlotIndex() const { return SelectedHotSlot; }

	FORCEINLINE bool GetIsOpenInventory() const { return IsOpenInventory; }
	FORCEINLINE TObjectPtr<UPlayerEquipmentUI> GetInventoryUI() const { return EquipmentUI; }
	
	void SetHotSlotIndex(const uint8 NewIndex);

	void SetHotSlotItemToPlayer(const uint8 PrevIndex, const uint8 NewIndex);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ChangeHotSlotAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ItemDropAction;
	
	// TODO: UI 관련은 공통 컴포넌트로 이전해도 무방해보임
	UPROPERTY(EditDefaultsOnly, Category="Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerEquipmentUI> EquipmentUIClass;
	
	TObjectPtr<UPlayerEquipmentUI> EquipmentUI;

	uint8 SelectedHotSlot = 0;

	bool IsOpenInventory = false;
	
	UFUNCTION()
	void ToggleInventory();
	
	uint8 GetNextSlot(const int8 MoveTo);
	
	UFUNCTION()
	void ChangeHotSlot(const FInputActionValue& Value);
	
	UFUNCTION()
	void DropItem();
};

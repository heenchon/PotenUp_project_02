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

	void Initialize();
	
private:
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ChangeHotSlotAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SetHotSlotAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ItemDropAction;
	
	// TODO: UI 관련은 공통 컴포넌트로 이전해도 무방해보임
	UPROPERTY(EditDefaultsOnly, Category="Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerEquipmentUI> EquipmentUIClass;
	
	TObjectPtr<UPlayerEquipmentUI> EquipmentUI;

	// TODO: 이런 정보는 PlayerController로 이전하는 것도 방법으로 보인다.
	// UI 관리는 사실 이 컴포넌트보다는 PC에서 해주는 게 맞다.
	uint8 SelectedHotSlot = 0;

	bool IsOpenInventory = false;
	
	UFUNCTION()
	void ToggleInventory();
	
	uint8 GetNextSlot(const int8 MoveTo);
	
	UFUNCTION()
	void ChangeHotSlot(const FInputActionValue& Value);
	
	UFUNCTION()
	void SetHotSlot(const FInputActionValue& Value);
	
	UFUNCTION()
	void DropItem();
};

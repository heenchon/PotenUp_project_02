#include "InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Widgets/HUD/PlayerEquipmentUI.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Player->InputComponent))
		{
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered
			, this, &ThisClass::ToggleInventory);
			EnhancedInputComponent->BindAction(ChangeHotSlotAction, ETriggerEvent::Triggered
			, this, &ThisClass::ChangeHotSlot);
		}
	}
}

void UInventoryComponent::ChangeHotSlot(const FInputActionValue& Value)
{
	const float NewValue = Value.Get<float>();
	SetHotSlotIndex(GetNextSlot(static_cast<int8>(NewValue)));
}

uint8 UInventoryComponent::GetNextSlot(const int8 MoveTo)
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		ABasePlayerState* PS = Player->GetPlayerState<ABasePlayerState>();
		check(PS);

		if (SelectedHotSlot + MoveTo < 0)
		{
			return PS->GetHotSlotCount() + (SelectedHotSlot + MoveTo);
		}

		if (SelectedHotSlot + MoveTo >= PS->GetHotSlotCount())
		{
			return SelectedHotSlot + MoveTo - PS->GetHotSlotCount();
		}
	}

	return SelectedHotSlot + MoveTo;
}

void UInventoryComponent::SetHotSlotIndex(const uint8 NewIndex)
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (Player->GetController()->IsA(ABasePlayerController::StaticClass()))
		{
			ABasePlayerController* PC =
				static_cast<ABasePlayerController*>(Player->GetController());
			
			if (UPlayerGameUI* GameUI = Cast<UPlayerGameUI>(PC->GetPlayerUI()))
			{
				GameUI->SetHotSlotIndex(SelectedHotSlot, NewIndex);
			}
		}
	}
	SelectedHotSlot = NewIndex;
}

void UInventoryComponent::ToggleInventory()
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if(!EquipmentUIClass)
		{
			return;
		}
		
		if (!IsValid(EquipmentUI))
		{
			EquipmentUI = CreateWidget<UPlayerEquipmentUI>(
				Cast<ABasePlayerController>(Player->GetController()), EquipmentUIClass);
		}

		// 인벤토리 실질적 토글
		if (IsOpenInventory)
		{
			EquipmentUI->RemoveFromParent();
		} else
		{
			EquipmentUI->AddToViewport();
		}

		// 현재 인벤토리 상태 설정
		IsOpenInventory = !IsOpenInventory;
	}
}
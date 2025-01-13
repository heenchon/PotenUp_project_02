#include "InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Game/BaseGameInstance.h"
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
	SetHotSlotItemToPlayer(-1, SelectedHotSlot);
}

void UInventoryComponent::ChangeHotSlot(const FInputActionValue& Value)
{
	const float NewValue = Value.Get<float>();
	// UI 관련 변화
	const uint8 PrevIndex = SelectedHotSlot;
	const uint8 NextIndex = GetNextSlot(static_cast<int8>(NewValue));
	
	// 실제 액터 반영
	SetHotSlotItemToPlayer(PrevIndex, NextIndex);
	SetHotSlotIndex(NextIndex);
}

uint8 UInventoryComponent::GetNextSlot(const int8 MoveTo)
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		const ABasePlayerState* PS = Player->GetPlayerState<ABasePlayerState>();
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
			// TODO: 관련 사항은 Controller 차원에서 Delegate를 걸어도 될 것 같은데...
			// 아직 Delegate 형식에 익숙치 않아서 TODO로 넘겨둠
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

void UInventoryComponent::SetHotSlotItemToPlayer(const uint8 PrevIndex, const uint8 NewIndex)
{
	if (GetOwner()->IsA(APlayerCharacter::StaticClass())) {
		APlayerCharacter* Player = static_cast<APlayerCharacter*>(GetOwner());
		ABasePlayerState* PS = static_cast<ABasePlayerState*>(Player->GetPlayerState());
		const UBaseGameInstance* GameInstance =
			static_cast<UBaseGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (PrevIndex != NewIndex)
		{
			const FItemInfoData ItemInfo = GameInstance->GetItemInfoList()[
					PS->GetPlayerInventoryList()[NewIndex].GetId()
				];
			
			Player->SetTestInteractiveItem(ItemInfo.GetShowItemActor() ? ItemInfo.GetShowItemActor() : nullptr);
		} else
		{
			Player->SetTestInteractiveItem(nullptr);
		}
	}
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
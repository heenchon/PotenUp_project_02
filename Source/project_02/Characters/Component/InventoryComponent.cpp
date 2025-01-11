#include "InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/HUD/PlayerEquipmentUI.h"

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
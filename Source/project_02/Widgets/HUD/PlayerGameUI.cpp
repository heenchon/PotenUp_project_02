#include "PlayerGameUI.h"

#include "../HUD/Module/StatusProgressBar.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "project_02/Widgets/Inventory/InventoryHotSlot.h"

void UPlayerGameUI::SetHotSlotIndex(const uint8 PrevIndex, const uint8 NextIndex)
{
	InventoryHotSlot->ChangeSelectedHotSlot(PrevIndex, NextIndex);
}

void UPlayerGameUI::SetHealthPercent(const uint8 Current, const uint8 Max)
{
	HealthBar->SetPercent( static_cast<float>(Current) / Max);
}

void UPlayerGameUI::SetHungerPercent(const uint8 Current, const uint8 Max)
{
	HungerBar->SetPercent( static_cast<float>(Current) / Max);
}

void UPlayerGameUI::SetThirstPercent(const uint8 Current, const uint8 Max)
{
	ThirstBar->SetPercent( static_cast<float>(Current) / Max);
}

void UPlayerGameUI::SetInteractiveUIStatus(AActor* Target)
{
	if (IsValid(Target))
	{
		InteractiveUI->SetVisibility(ESlateVisibility::Visible);
		InteractiveUITitle->SetText(FText::FromString(Target->GetName()));
	} else
	{
		InteractiveUI->SetVisibility(ESlateVisibility::Hidden);
	}
}



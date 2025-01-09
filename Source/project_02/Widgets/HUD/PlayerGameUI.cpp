#include "PlayerGameUI.h"

#include "../HUD/Module/StatusProgressBar.h"

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


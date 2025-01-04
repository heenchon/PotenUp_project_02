#include "PlayerGameUI.h"

#include "StatusProgressBar.h"

void UPlayerGameUI::SetHealthPercent(const uint8 Current, const uint8 Max)
{
	HealthBar->SetPercent( static_cast<float>(Current) / Max);
}

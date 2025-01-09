#include "PlayerRespawnUI.h"

#include "Components/Button.h"
#include "project_02/Player/BasePlayerController.h"

void UPlayerRespawnUI::NativeOnInitialized()
{
	RespawnButton->OnClicked.AddDynamic(this, &ThisClass::OnClickRespawnButton);
}

void UPlayerRespawnUI::OnClickRespawnButton()
{
	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer()))
	{
		PC->Respawn();
	}
}

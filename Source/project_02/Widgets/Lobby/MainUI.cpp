#include "MainUI.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "project_02/Game/RaftGameMode.h"

void UMainUI::NativeOnInitialized()
{
	NewGameButton->OnClicked.AddDynamic(
	this, &ThisClass::OnClickNewGameButton);
	ContinueGameButton->OnClicked.AddDynamic(
		this, &ThisClass::OnClickContinueGameButton);
}

void UMainUI::OnClickNewGameButton()
{
	GetWorld()->GetAuthGameMode<ARaftGameMode>()->GoToMainLevel();
	RemoveFromParent();
}

void UMainUI::OnClickContinueGameButton()
{
	UUserWidget* NewWidget = CreateWidget(this, SubWidgetClass);
	SubModuleOverlay->AddChild(NewWidget);
}

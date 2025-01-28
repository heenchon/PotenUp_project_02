#include "MainUI.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "project_02/Game/RaftGameMode.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	NewGameButton->OnClicked.AddDynamic(
	this, &ThisClass::OnClickNewGameButton);
	ContinueGameButton->OnClicked.AddDynamic(
		this, &ThisClass::OnClickContinueGameButton);
}

void UMainUI::OnClickNewGameButton()
{
	UUserWidget* NewWidget = CreateWidget(this, SubWidgetClass);
	SubModuleOverlay->AddChild(NewWidget);
}

void UMainUI::OnClickContinueGameButton()
{
	GetWorld()->GetAuthGameMode<ARaftGameMode>()->StartPlayGame("TestSlot");
	RemoveFromParent();
}

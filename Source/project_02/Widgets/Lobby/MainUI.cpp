#include "MainUI.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Module/MapCreate/CreateNewMap.h"
#include "Module/MapSelect/SelectMapList.h"

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
	UCreateNewMap* NewWidget = CreateWidget<UCreateNewMap>(this, CreateNewMapClass);
	SubModuleOverlay->ClearChildren();
	SubModuleOverlay->AddChild(NewWidget);
}

void UMainUI::OnClickContinueGameButton()
{
	USelectMapList* NewWidget = CreateWidget<USelectMapList>(this, SelectMapListClass);
	SubModuleOverlay->ClearChildren();
	SubModuleOverlay->AddChild(NewWidget);
}

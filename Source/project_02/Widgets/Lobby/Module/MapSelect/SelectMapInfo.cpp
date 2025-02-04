#include "SelectMapInfo.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "project_02/Game/RaftGameMode.h"

void USelectMapInfo::NativeConstruct()
{
	ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::OnClickConfirm);
}

void USelectMapInfo::OnClickConfirm()
{
	ARaftGameMode* GM = GetWorld()->GetAuthGameMode<ARaftGameMode>();
	if (!GM)
	{
		return;
	}
	
	GM->StartPlayGame(MapName->GetText().ToString());
}

void USelectMapInfo::SetMapName(const FString& NewMapName) const
{
	MapName->SetText(FText::FromString(NewMapName));
}

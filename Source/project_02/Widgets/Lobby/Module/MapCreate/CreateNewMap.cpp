#include "CreateNewMap.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Game/RaftGameMode.h"
#include "project_02/Game/RaftSaveList.h"

void UCreateNewMap::NativeConstruct()
{
	MapNameInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnCommitText);
	MapNameInput->OnTextChanged.AddDynamic(this, &ThisClass::OnChangeText);
	WarningText->SetVisibility(ESlateVisibility::Hidden);
	ConfirmButton->SetIsEnabled(false);
	ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::OnClickConfirm);
}

void UCreateNewMap::OnChangeText(const FText& Text)
{
	// 아에 공백이 아니라면 활성화 처리
	ConfirmButton->SetIsEnabled(Text.ToString().Len() > 0);
}

void UCreateNewMap::OnCommitText(const FText& Text,
	ETextCommit::Type CommitMethod)
{
	// Commit이 Focus를 없애면서 일어나는 행위기 때문에
	// Enter를 누르면 focus out되면서 한번, commit 때문에 한번 총 2번이 발생한다.
	// 그렇기 때문에 우선적으로 Enter 처리되는 그 상황에만 관련된 내용을 처리한다.
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		const URaftSaveList* SaveData = Cast<URaftSaveList>(
					UGameplayStatics::LoadGameFromSlot("SaveList", 0));
		if (!SaveData)
		{
			WarningText->SetVisibility(ESlateVisibility::Hidden);
			return;
		}
		
		const bool IsFindDuplicatedMap = SaveData->MapNameList.IndexOfByPredicate([&](const FSaveData& Data)
		{
			return Data.MapName == Text.ToString();
		}) != -1;
		
		if (IsFindDuplicatedMap)
		{
			WarningText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		} else
		{
			WarningText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UCreateNewMap::OnClickConfirm()
{
	const URaftSaveList* SaveData = Cast<URaftSaveList>(
					UGameplayStatics::LoadGameFromSlot("SaveList", 0));
	if (!SaveData)
	{
		ARaftGameMode* GM = GetWorld()->GetAuthGameMode<ARaftGameMode>();
		if (!GM)
		{
			return;
		}
		GM->StartPlayGame(MapNameInput->GetText().ToString());
	}
	
	const bool IsFindDuplicatedMap = SaveData->MapNameList.IndexOfByPredicate([&](const FSaveData& Data)
	{
		return Data.MapName == MapNameInput->GetText().ToString();
	}) != -1;
	
	// 확정 시에도 재검증은 해주면 좋다.
	if (IsFindDuplicatedMap)
	{
		WarningText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	} else
	{
		ARaftGameMode* GM = GetWorld()->GetAuthGameMode<ARaftGameMode>();
		if (!GM)
		{
			return;
		}
		GM->StartPlayGame(MapNameInput->GetText().ToString());
	}
}

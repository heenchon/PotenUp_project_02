#include "CreateNewMap.h"

#include "Components/EditableTextBox.h"

void UCreateNewMap::NativeConstruct()
{
	MapNameInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnCommitText);
}

void UCreateNewMap::OnCommitText(const FText& Text,
	ETextCommit::Type CommitMethod)
{
	
}

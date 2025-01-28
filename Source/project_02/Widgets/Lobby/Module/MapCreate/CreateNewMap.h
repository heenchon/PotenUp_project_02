#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateNewMap.generated.h"


class UEditableTextBox;

UCLASS()
class PROJECT_02_API UCreateNewMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> MapNameInput;

	UFUNCTION()
	void OnCommitText(const FText& Text, ETextCommit::Type CommitMethod);
};

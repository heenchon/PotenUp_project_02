#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateNewMap.generated.h"


class UButton;
class UTextBlock;
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
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WarningText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;
	
	UFUNCTION()
	void OnCommitText(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void OnChangeText(const FText& Text);

	UFUNCTION()
	void OnClickConfirm();
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UButton;

UCLASS()
class PROJECT_02_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;
	
	UFUNCTION()
	void OnClickStartGameButton();
};

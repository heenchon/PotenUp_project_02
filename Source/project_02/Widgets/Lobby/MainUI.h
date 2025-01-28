#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UOverlay;
class UButton;

UCLASS()
class PROJECT_02_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NewGameButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueGameButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> SubModuleOverlay;

	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> SubWidgetClass;
	
	UFUNCTION()
	void OnClickNewGameButton();

	UFUNCTION()
	void OnClickContinueGameButton();
};

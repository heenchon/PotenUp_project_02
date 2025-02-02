#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UCreditList;
class USelectMapList;
class UCreateNewMap;
class UOverlay;
class UButton;

UCLASS()
class PROJECT_02_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NewGameButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueGameButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreditButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> SubModuleOverlay;

	UPROPERTY()
	TObjectPtr<UCreditList> CreditList;
	
	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCreditList> CreditListClass;
	
	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCreateNewMap> CreateNewMapClass;
	
	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<USelectMapList> SelectMapListClass;
	
	UFUNCTION()
	void OnClickNewGameButton();

	UFUNCTION()
	void OnClickContinueGameButton();
	
	UFUNCTION()
	void OnClickCreditGameButton();
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerRespawnUI.generated.h"

class UButton;

UCLASS()
class PROJECT_02_API UPlayerRespawnUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RespawnButton;
	
	UFUNCTION()
	void OnClickRespawnButton();
};

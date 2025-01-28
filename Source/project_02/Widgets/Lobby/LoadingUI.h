#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUI.generated.h"

class UWidgetAnimation;

UCLASS()
class PROJECT_02_API ULoadingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayEnd();

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;
	
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	FWidgetAnimationDynamicEvent EndDelegate;
	
	UFUNCTION()
	void OnFinishFadeOut();
};

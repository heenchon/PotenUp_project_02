#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusProgressBar.generated.h"

class UProgressBar;
class UImage;
struct FSlateBrush;
struct FProgressBarStyle;

UCLASS()
class PROJECT_02_API UStatusProgressBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPercent(const float NewValue);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StatusBar;
	
	virtual void NativePreConstruct() override;
	
private:
	UPROPERTY()
	float Percent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options", meta = (AllowPrivateAccess = true))
	FSlateBrush ThumbnailOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options", meta = (AllowPrivateAccess = true))
	FProgressBarStyle StatusBarStyleOptions;
};

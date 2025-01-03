#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusProgressBar.generated.h"

class UProgressBar;

UCLASS()
class PROJECT_02_API UStatusProgressBar : public UUserWidget
{
	GENERATED_BODY()

public: 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StatusBar;
};

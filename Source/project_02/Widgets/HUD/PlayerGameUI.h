#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerGameUI.generated.h"

class UStatusProgressBar;

UCLASS()
class PROJECT_02_API UPlayerGameUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthPercent(const uint8 Current, const uint8 Max);
	void SetHungerPercent(const uint8 Current, const uint8 Max);
	void SetThirstPercent(const uint8 Current, const uint8 Max);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusProgressBar> HungerBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusProgressBar> ThirstBar;
};

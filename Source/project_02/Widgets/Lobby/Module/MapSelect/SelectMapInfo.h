#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapInfo.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class PROJECT_02_API USelectMapInfo : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMapName(const FString& NewMapName) const;

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MapName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	UFUNCTION()
	void OnClickConfirm();
};

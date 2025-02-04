#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapList.generated.h"

class UScrollBox;
class USelectMapInfo;

UCLASS()
class PROJECT_02_API USelectMapList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> RequireList;

	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<USelectMapInfo> SelectInfoClass;
};
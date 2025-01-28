#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapList.generated.h"

class USelectMapInfo;
class UVerticalBox;

UCLASS()
class PROJECT_02_API USelectMapList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> RequireList;

	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<USelectMapInfo> SelectInfoClass;
};

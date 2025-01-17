#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

struct FItemInfoData;

UCLASS()
class PROJECT_02_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UBaseGameInstance();

public:
	FORCEINLINE TArray<FItemInfoData> GetItemInfoList() const{ return ItemInfoList; };

protected:
	virtual void Init() override;
	
private:
	UPROPERTY()
	TArray<FItemInfoData> ItemInfoList;
};

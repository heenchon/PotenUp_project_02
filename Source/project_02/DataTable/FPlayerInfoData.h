#pragma once

#include "CoreMinimal.h"
#include "FPlayerInfoData.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_02_API FPlayerInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	uint8 MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	uint8 MaxHunger;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	uint8 MaxThirst;
};

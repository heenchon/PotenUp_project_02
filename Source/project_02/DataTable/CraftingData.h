#pragma once

#include "CoreMinimal.h"
#include "CraftingData.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_02_API FCraftingData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// ItemKey 와 갯수 정보만 담음
	// 만약 특정 내구도 정보나 그런게 필요하면 column 추가 필요
	UPROPERTY(EditDefaultsOnly)
	TMap<uint32, uint32> Recipe;
};

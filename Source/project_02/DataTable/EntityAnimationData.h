#pragma once

#include "CoreMinimal.h"
#include "EntityAnimationData.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_02_API FEntityAnimationData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	TSoftObjectPtr<UAnimMontage> DiedAnimation;
};

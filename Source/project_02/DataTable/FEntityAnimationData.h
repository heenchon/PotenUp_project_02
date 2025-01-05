#pragma once

#include "CoreMinimal.h"
#include "FEntityAnimationData.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_02_API FEntityAnimationData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	TObjectPtr<UAnimMontage> DiedAnimation;
};

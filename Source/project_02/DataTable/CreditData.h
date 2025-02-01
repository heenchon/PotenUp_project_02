#pragma once

#include "CoreMinimal.h"
#include "CreditData.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_02_API FCreditData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FString DeveloperName;

	UPROPERTY()
	FDateTime CommitDate;

	UPROPERTY()
	FString CommitDesc;
};

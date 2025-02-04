#pragma once

#include "CoreMinimal.h"
#include "project_02/DataTable/CreditData.h"
#include "UObject/Object.h"
#include "CreditObject.generated.h"

UCLASS()
class PROJECT_02_API UCreditObject : public UObject
{
	GENERATED_BODY()
public:
	FCreditData Data;
};

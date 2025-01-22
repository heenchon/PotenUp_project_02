#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "project_02/DataTable/BuildData.h"
#include "BuildingInstance.generated.h"

UCLASS()
class PROJECT_02_API UBuildingInstance : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();

	void UpdateBuildMetaData(const FVector& Pos, const bool IsRemove = false);

	FORCEINLINE TMap<FVector, FBuildData> GetRaftBuildMetaData() const { return RaftBuildMetaData; }
private:
	TMap<FVector, FBuildData> RaftBuildMetaData;
};

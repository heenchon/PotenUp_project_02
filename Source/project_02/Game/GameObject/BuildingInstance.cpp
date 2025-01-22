#include "BuildingInstance.h"
#include "project_02/DataTable/BuildData.h"

void UBuildingInstance::Initialize()
{
	// 여기는 저장된 정보가 없는 경우에 선언하는 방식
	FBuildData BuildData;
	BuildData.BlockType = EBlockType::Wood;
	
	RaftBuildMetaData.Add(FVector::ZeroVector, BuildData);
}

void UBuildingInstance::UpdateBuildMetaData(const FVector& Pos, const bool IsRemove)
{
	if (IsRemove)
	{
		RaftBuildMetaData.Remove(Pos);
	} else
	{
		FBuildData BuildData;
		// TODO: 현재는 단순 추가이지만 추후 업데이트 로직도 넣을 필요가 있다.
		
		RaftBuildMetaData.Add(Pos, BuildData);
	}
}

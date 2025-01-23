#include "RaftGameState.h"

void ARaftGameState::SetWind(FVector newDir, float newStrength)
{
	WindDirection = newDir;
	WindStrength = newStrength;
}

void ARaftGameState::UpdateBuildMetaData(const FVector& Pos, ABuildingActor* Build, const bool IsRemove)
{
	if (IsRemove)
	{
		RaftBuildMetaData.Remove(Pos);
		RaftBuildPointerData.Remove(Pos);
	} else
	{
		FBuildData BuildData;
		BuildData.BlockType = EBlockType::Wood;
		// TODO: 현재는 단순 추가이지만 추후 업데이트 로직도 넣을 필요가 있다.
		RaftBuildMetaData.Add(Pos, BuildData);
		RaftBuildPointerData.Add(Pos, Build);
	}
}

void ARaftGameState::BeginPlay()
{
}

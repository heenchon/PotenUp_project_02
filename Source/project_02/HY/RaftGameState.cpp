#include "RaftGameState.h"
#include "project_02/Game/GameObject/BuildingInstance.h"

void ARaftGameState::SetWind(FVector newDir, float newStrength)
{
	WindDirection = newDir;
	WindStrength = newStrength;
}

void ARaftGameState::BeginPlay()
{
	// 런타임에서 새로 만드는 인스턴스. 단 한번만 만들어주면 된다.
	BuildingInstance = NewObject<UBuildingInstance>();
	BuildingInstance->Initialize();
}

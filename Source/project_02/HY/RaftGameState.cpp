#include "RaftGameState.h"

void ARaftGameState::SetWind(FVector newDir, float newStrength)
{
	WindDirection = newDir;
	WindStrength = newStrength;
}

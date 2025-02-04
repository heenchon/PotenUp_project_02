#include "RaftGameState.h"

#include "Shark/SharkAI.h"

void ARaftGameState::SetWind(FVector newDir, float newStrength)
{
	WindDirection = newDir;
	WindStrength = newStrength;
}

void ARaftGameState::SpawnShark()
{
	if (Shark)
	{
		return;
	}

	if (SharkClass)
	{
		Shark = GetWorld()->SpawnActor<ASharkAI>(SharkClass);
	}
}

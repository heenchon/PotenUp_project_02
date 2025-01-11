#include "SRunAwayState.h"
#include "../SharkAI.h"

void SRunAwayState::Enter(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Entering Idle State"));
}

void SRunAwayState::Update(ASharkAI* Shark)
{
	
}

void SRunAwayState::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Exiting Idle State"));
}
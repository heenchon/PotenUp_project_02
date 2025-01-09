#include "SIdleState.h"
#include "../SharkAI.h"

void SIdleState::Enter(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Entering Idle State"));
}

void SIdleState::Update(ASharkAI* Shark)
{
	
}

void SIdleState::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Exiting Idle State"));
}
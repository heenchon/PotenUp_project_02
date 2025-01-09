#include "SMoveToRaftState.h"
#include "../SharkAI.h"

void SMoveToRaftState::Enter(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Entering Idle State"));
}

void SMoveToRaftState::Update(ASharkAI* Shark)
{
	
}

void SMoveToRaftState::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Exiting Idle State"));
}

#include "SAttackRaftState.h"

void SAttackRaftState::Enter(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Entering Idle State"));
}

void SAttackRaftState::Update(ASharkAI* Shark)
{
	
}

void SAttackRaftState::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("Exiting Idle State"));
}
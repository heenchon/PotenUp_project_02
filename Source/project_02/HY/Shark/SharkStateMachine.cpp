#include "SharkStateMachine.h"
#include "SharkAI.h"
#include "SharkState.h"


USharkStateMachine::USharkStateMachine() : CurState(nullptr) {}

void USharkStateMachine::ChangeState(UObject* newState, ASharkAI* Shark)
{
	if (CurState)
	{
		ISharkState* TempState = Cast<ISharkState>(CurState);
		if (TempState) TempState->Exit(Shark);
	}

	CurState = newState;
	if (CurState)
	{
		ISharkState* NewState = Cast<ISharkState>(CurState);
		if (NewState) NewState->Enter(Shark, this);
	}
}

void USharkStateMachine::Update(ASharkAI* Shark ,float DeltaTime)
{
	if (CurState) {
		ISharkState* State = Cast<ISharkState>(CurState);
		if (State)
		{
			State->Update(Shark, DeltaTime);
		}
	}
}



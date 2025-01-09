#include "SharkStateMachine.h"

#include "SharkAI.h"
#include "SharkState.h"

USharkStateMachine::USharkStateMachine() : CurState(nullptr) {}

void USharkStateMachine::ChangeState(UObject* newState, ASharkAI* Shark)
{
	if (CurState)
	{
		ISharkState* CurStateInterface = Cast<ISharkState>(CurState);
		if (CurStateInterface) CurStateInterface->Exit(Shark);
	}

	CurState = newState;
	if (CurState)
	{
		ISharkState* NewStateInterface = Cast<ISharkState>(CurState);
		if (NewStateInterface) NewStateInterface->Enter(Shark);
	}
}

void USharkStateMachine::Update(ASharkAI* Shark ,float DeltaTime)
{
	if (CurState) {
		ISharkState* CurStateInterface = Cast<ISharkState>(CurState);
		if (CurStateInterface)
		{
			CurStateInterface->Update(Shark, DeltaTime);
		}
	}
}

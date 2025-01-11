#pragma once

#include "CoreMinimal.h"
#include "../Shark/SharkState.h"

class SharkStateMachine
{
public:
	SharkStateMachine();
	~SharkStateMachine();

	ISharkState* CurState;

	void ChangeState(ISharkState* newState, ASharkAI* Shark);
	void Update(ASharkAI* Shark);
};

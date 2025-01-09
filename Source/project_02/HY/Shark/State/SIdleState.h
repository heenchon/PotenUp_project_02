#pragma once
#include "../SharkState.h"

class SIdleState: public ISharkState
{
public:
	virtual void Enter(ASharkAI* Shark) override;
	virtual void Update(ASharkAI* Shark) override;
	virtual void Exit(ASharkAI* Shark) override;
};

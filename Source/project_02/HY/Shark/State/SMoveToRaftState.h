#pragma once
#include "../SharkState.h"

class SMoveToRaftState : ISharkState
{
	public:
	virtual void Enter(ASharkAI* Shark) override;
	virtual void Update(ASharkAI* Shark) override;
	virtual void Exit(ASharkAI* Shark) override;
};

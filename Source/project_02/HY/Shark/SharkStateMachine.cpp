// #include "SharkStateMachine.h"
//
// void SharkStateMachine::ChangeState(ISharkState* newState, ASharkAI* Shark)
// {
// 	if (CurState)
// 	{
// 		CurState->Exit(Shark);
// 		delete CurState;
// 	}
//
// 	CurState = newState;
// 	if (CurState) CurState->Enter(Shark);
// }
//
// void SharkStateMachine::Update(ASharkAI* Shark)
// {
// 	if (CurState) CurState->Update(Shark);
// }

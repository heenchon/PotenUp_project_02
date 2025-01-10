// Fill out your copyright notice in the Description page of Project Settings.
#include "SharkIdle.h"
#include "../SharkAI.h"
// #include  "../SharkStateMachine.h"
#include "../ESharkState.h"

void USharkIdle::Enter(ASharkAI* Shark, USharkStateMachine* FSM)
{
	UE_LOG(LogTemp, Warning, TEXT("IDLE 시작"));
}

void USharkIdle::Update(ASharkAI* Shark, float DeltaTime)
{
	// UE_LOG(LogTemp, Display, TEXT("기회를 엿보는 중..."));
	CurTime += DeltaTime;
	if (CurTime > AttackDuration)
	{
		UE_LOG(LogTemp, Display, TEXT("배이동 호출!"))
		// Shark->StateMachine->ChangeState(ESharkState::MoveToPlayer,Shark);
	}
}

void USharkIdle::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("IDLE 종료"));
}








// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAttackingRaft.h"

void USharkAttackingRaft::Enter(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("배공격 시작"));
}

void USharkAttackingRaft::Update(ASharkAI* Shark, float DeltaTime)
{
	UE_LOG(LogTemp, Display, TEXT("와그작와그작"));
}

void USharkAttackingRaft::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("배공격 종료"));
}
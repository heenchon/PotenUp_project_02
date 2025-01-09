// Fill out your copyright notice in the Description page of Project Settings.
#include "SharkMovingToRaft.h"
#include "../SharkAI.h"

void USharkMovingToRaft::Enter(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("배 이동 시작"));
	// TargetLocation = Shark->Player().GetActorLocation();
}

void USharkMovingToRaft::Update(ASharkAI* Shark, float DeltaTime)
{
	UE_LOG(LogTemp, Display, TEXT("가는 중..."));
	FVector MyLocation = Shark->GetActorLocation();
	FVector Dir = TargetLocation - MyLocation;
	Shark->SetActorLocation(MyLocation+Dir*DeltaTime*Shark->SharkAttackSpeed);
}

void USharkMovingToRaft::Exit(ASharkAI* Shark)
{
	UE_LOG(LogTemp, Warning, TEXT("배 이동 종료"));
}
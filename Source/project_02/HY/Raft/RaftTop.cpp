// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftTop.h"
#include "../RaftGameState.h"

URaftTop::URaftTop()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URaftTop::BeginPlay()
{
	Super::BeginPlay();
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();
	if (RaftGameState)
	{
		WindDirection = RaftGameState->WindDirection;
		WindStrength = RaftGameState->WindStrength;
	}
}

void URaftTop::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetWorldLocation(GetComponentLocation()+WindDirection*DeltaTime*WindStrength*SailStrength);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftGameState.h"

void ARaftGameState::SetWind(FVector newDir, float newStrength)
{
	WindDirection = newDir;
	WindStrength = newStrength;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftTop.h"

URaftTop::URaftTop()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URaftTop::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetWorldLocation(GetComponentLocation()+WindDirection*DeltaTime*WindStrength*SailStrength);
}

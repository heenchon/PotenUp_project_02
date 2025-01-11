// Fill out your copyright notice in the Description page of Project Settings.


#include "RafaTop.h"

URafaTop::URafaTop()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URafaTop::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("컴포넌트 틱"));
	SetWorldLocation(GetComponentLocation()+WindDirection*DeltaTime*WindStrength*SailStrength);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SailComp.h"
#include "Raft.h"

USailComp::USailComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	MinSailStrength = 1.0f / MaxSailStrength;
}

void USailComp::BeginPlay()
{
	Super::BeginPlay();
	Raft = Cast<ARaft>(GetOwner());
	// UE_LOG(LogTemp, Warning, TEXT("래프트 가져왔나요? %s"), *(Raft->GetName()));
	WindDirection = Raft->WindDirection;
	WindStrength = Raft->WindStrength;
	// UE_LOG(LogTemp,Warning,TEXT("돛의 Min: %f"), MinSailStrength);
}

void USailComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MyDirection = this->GetComponentRotation().Vector();
	ChangeStrength(CompareDirection(MyDirection, WindDirection));
	// UE_LOG(LogTemp, Warning, TEXT("포워드 벡터: %s"), *MyDirection.ToString());
}


void USailComp::ChangeStrength(float myStrength)
{
	Raft->SailStrength = myStrength;
}

float USailComp::CompareDirection(FVector3d myDir, FVector3d windDir)
{
	float ForceMultiplier = FMath::Clamp(FVector::DotProduct(myDir, windDir), MinSailStrength, 1.0f);
	// UE_LOG(LogTemp, Warning, TEXT("바람 방향: %s  가중값: %f"), *windDir.ToString(), ForceMultiplier);
	return (MaxSailStrength * ForceMultiplier);
}

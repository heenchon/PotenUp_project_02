// Fill out your copyright notice in the Description page of Project Settings.


#include "SailComp.h"
#include "../RaftGameState.h"
#include "../Raft/Raft.h"

USailComp::USailComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	MinSailStrength = 1.0f / MaxSailStrength;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USailComp::BeginPlay()
{
	Super::BeginPlay();
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();
	if (RaftGameState)
	{
		WindDirection = RaftGameState->WindDirection;
		WindStrength = RaftGameState->WindStrength;
	}
	Raft = Cast<ARaft>(GetOwner());
	// UE_LOG(LogTemp,Warning,TEXT("돛의 Min: %f"), MinSailStrength);
	SailToggle();
}

void USailComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MyDirection = this->GetComponentRotation().Vector();
	if (bSailOn) ChangeStrength(CompareDirection(MyDirection, WindDirection));
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

void USailComp::SailToggle()
{
	if (bSailOn)
	{
		bSailOn = false;
		//TODO: 임시로 애니메이션 대신 스케일 조정
		SetRelativeScale3D(FVector(0.05, 0.6, 0.05));
	}
	else
	{
		bSailOn = true;
		SetRelativeScale3D(FVector(0.05, 0.6, 0.75));
	}
}

void USailComp::RotateSail()
{
	FRotator curRot = GetComponentRotation();
	double newYaw= curRot.Yaw + RotateAddValue;
	// SetWorldRotation(FRotator(curRot.Pitch, newYaw,curRot.Roll));
	// UE_LOG(LogTemp,Warning,TEXT("Pitch %f  Yaw %f  Roll %f"),curRot.Pitch,curRot.Yaw,curRot.Roll);
}

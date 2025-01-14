// Fill out your copyright notice in the Description page of Project Settings.

#include "PaddleTest.h"
#include "project_02/HY/Raft/Raft.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APaddleTest::APaddleTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APaddleTest::BeginPlay()
{
	Super::BeginPlay();
	if (!Player)
	{
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
		// UE_LOG(LogTemp,Warning,TEXT("플레이어는? %s"),*Player->GetName());
	}
}

// Called every frame
void APaddleTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int APaddleTest::PaddlingStart()
{
	// UE_LOG(LogTemp, Error, TEXT("Paddling..."));

	//Yaw값만을 반영하는 플레이어 정면
	FRotator PlayerRotator = Player->GetActorRotation();
	FVector PlayerForwardVector = FRotator(0.0f, PlayerRotator.Yaw,0.0f).Vector();
	// UE_LOG(LogTemp, Warning, TEXT("플레이어 방향: %s"), *PlayerForwardVector.ToString());

	//원본 바람 방향 백업
	WindOriginDir = Raft->WindDirection;

	//기존 바람 방향에 노 방향을 더한 새 방향
	Raft->WindDirection = PlayerForwardVector + WindOriginDir;
	// UE_LOG(LogTemp, Warning, TEXT("최종 이동 방향: %s"),*(Raft->WindDirection .ToString()));
	
	return 0;
}

int APaddleTest::PaddlingEnd()
{
	// UE_LOG(LogTemp, Warning, TEXT("stop"));

	//바람 방향 되돌리기
	Raft->WindDirection = WindOriginDir;

	return 0;
}




